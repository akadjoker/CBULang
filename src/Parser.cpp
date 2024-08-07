#include "pch.h"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"
#include "Utils.hpp"

Parser::Parser(){}

    
void Parser::Load(const std::vector<Token> &tokens)
{
this->tokens = tokens;
current = 0;
panicMode = false;
countBegins = 0;
countEnds = 0;
}

Parser::~Parser()
{
   
}

std::shared_ptr<Expr> Parser::result()
{

    return   logic_or();
       

}

std::shared_ptr<Stmt> Parser::parse()
{
    try
    {
        return programStmt();
    }
    catch(const std::exception& e)
    {
        synchronize();
        Log(2, e.what());
        return nullptr;
    }
    
   
}

void Parser::clear()
{

    tokens.clear();
    current = 0;
    panicMode = false;
    countBegins = 0;
    countEnds = 0;
}

bool Parser::match(std::vector<TokenType> types)
{
    for (auto type : types)
    {
        if (check(type))
        {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::match(TokenType type)
{
    if (check(type))
    {
        advance();
        return true;
    }

    return false;
}

Token Parser::consume(TokenType type,const std::string &message)
{
    if (check(type))
    {
      return   advance();
    }
    else
    {
        Error(tokens[current],message+" have '"+tokens[current].lexeme+"'");
        return peek();
    }
}

bool Parser::check(TokenType type)
{
    if (isAtEnd()) return false;
    return tokens[current].type == type;
}

bool Parser::isAtEnd()
{
    return tokens[current].type == TokenType::END_OF_FILE;
}

void Parser::synchronize()
{

    advance();
    while(!isAtEnd())
    {
        if (tokens[current].type == TokenType::SEMICOLON)
        {
            advance();
            return;
        }
        advance();
    }
}

Token Parser::advance()
{
    if (!isAtEnd()) current++;
    return previous();
}

Token Parser::peek()
{
    return tokens[current];
}

Token Parser::previous()
{
    return tokens[current - 1];
}

Token Parser::lookAhead()
{
    if (current + 1 >= (int)tokens.size()) return previous();
    return tokens[current + 1];
}



void Parser::Error(const Token &token,const std::string &message)
{
    panicMode = true;
    int line = token.line;
    std::string text =message+ " at line: " +std::to_string(line);
    Log(2, text.c_str());
    synchronize();

    throw FatalException(text);
}

void Parser::Warning(const Token &token,const std::string &message)
{
    int line = token.line;
    std::string text =message+ " at line: " +std::to_string(line);
    Log(1, text.c_str());
}

std::shared_ptr<Expr> Parser::expression()
{
    return  assignment();
}

std::shared_ptr<Expr> Parser::assignment()
{
    std::shared_ptr<Expr> expr = logic_or();

    // std::cout<<"Assign: "<< expr->toString()<<" type: "<<peek().toString() << std::endl;
    Token name = previous();
    if (match(TokenType::EQUAL))
    {
         std::shared_ptr<Expr> value = assignment();
         if (expr->getType() == ExprType::VARIABLE)
         {
            return  std::make_shared<AssignExpr>(name, value);
         }

    } else 
    if (match(TokenType::PLUS_EQUAL))
    {
         std::shared_ptr<Expr> value = assignment();
         if (expr->getType() == ExprType::VARIABLE)
         {
            Token token = Token(TokenType::PLUS_EQUAL, name.lexeme,name.literal, name.line);
            std::shared_ptr<Expr> addition = std::make_shared<BinaryExpr>(expr, value, token);
            return std::make_shared<AssignExpr>(name, addition);
         }

    } else if (match(TokenType::MINUS_EQUAL))
    {
         std::shared_ptr<Expr> value = assignment();
         if (expr->getType() == ExprType::VARIABLE)
         {
            Token token = Token(TokenType::MINUS_EQUAL, name.lexeme,name.literal, name.line);
            std::shared_ptr<Expr> addition = std::make_shared<BinaryExpr>(expr, value, token);
            return std::make_shared<AssignExpr>(name, addition);
            
         }

    } else if (match(TokenType::STAR_EQUAL))
    {
         std::shared_ptr<Expr> value = assignment();
         if (expr->getType() == ExprType::VARIABLE)
         {
            Token token = Token(TokenType::STAR_EQUAL, name.lexeme,name.literal, name.line);
            std::shared_ptr<Expr> addition = std::make_shared<BinaryExpr>(expr, value, token);
            return std::make_shared<AssignExpr>(name, addition);
           
         }

    } else if (match(TokenType::SLASH_EQUAL))
    {
         std::shared_ptr<Expr> value = assignment();
         if (expr->getType() == ExprType::VARIABLE)
         {

            Token token = Token(TokenType::SLASH_EQUAL, name.lexeme,name.literal, name.line);
            std::shared_ptr<Expr> addition = std::make_shared<BinaryExpr>(expr, value, token);
            return std::make_shared<AssignExpr>(name, addition); 
           
         }

    }
    return expr;
}


std::shared_ptr<Expr> Parser::logic_or()
{
    std::shared_ptr<Expr> expr = logic_and();

    while (match(TokenType::OR))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = logic_and();
        expr = std::make_shared<LogicalExpr>(expr, right, op);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::logic_and()
{
    std::shared_ptr<Expr> expr = logic_xor();

    while (match(TokenType::AND))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = logic_xor();
        expr = std::make_shared<LogicalExpr>(expr, right, op);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::logic_xor()
{
    std::shared_ptr<Expr> expr = equality();

    while (match(TokenType::XOR))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = equality();
        expr = std::make_shared<LogicalExpr>(expr, right, op);
    }
    return expr;
}


std::shared_ptr<Expr> Parser::equality()
{
    std::shared_ptr<Expr> expr = comparison();
    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL}))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = comparison();
        expr = std::make_shared<BinaryExpr>(expr, right, op);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::comparison()
{
    std::shared_ptr<Expr> expr = term();
    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL}))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = term();
        expr = std::make_shared<BinaryExpr>(expr, right, op);
    }
    return expr;
}



std::shared_ptr<Expr> Parser::term()
{
    std::shared_ptr<Expr> expr = factor();
    while (match({TokenType::MINUS, TokenType::PLUS}))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = factor();
        expr = std::make_shared<BinaryExpr>(expr, right, op);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::factor()
{
    std::shared_ptr<Expr> expr = power();//^
    while (match({TokenType::STAR, TokenType::SLASH, TokenType::MOD}))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = power();//^
        expr = std::make_shared<BinaryExpr>(expr, right, op);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::power()
{
    std::shared_ptr<Expr> expr = unary();
    while (match(TokenType::POWER))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        
   
        expr = std::make_shared<BinaryExpr>(expr, right, op);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::unary()
{
    if (match({TokenType::BANG, TokenType::MINUS, TokenType::NOT, TokenType::INC, TokenType::DEC}))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        bool isPrefix = (op.type == TokenType::INC || op.type == TokenType::DEC);
        return std::make_shared<UnaryExpr>(right, op, isPrefix);
    }
    return call();
}

std::shared_ptr<Expr> Parser::call()
{

    std::shared_ptr<Expr> expr = primary();
    while(true)
    {
        if (match(TokenType::IDFUNCTION))
        {
            return functionCall();
        } else 
        if (match(TokenType::IDPROCESS))       
        {
            return processCall();
        }  else    if (match(TokenType::IDNATIVE))
        {
            return   nativeFunctionCall();
        }
        else
        {
            break;
        }
    }
    return expr;
}


std::shared_ptr<Expr> Parser::primary()
{
    if (match(TokenType::NOW))
    {
       return std::make_shared<NowExpr>();
    }



    if (match(TokenType::FALSE))
    {
        
          return  Factory::Instance().createBoolLiteral(false);
    }
    if (match(TokenType::TRUE))
    {
          
          return Factory::Instance().createBoolLiteral(true);
   
    }
    if (match(TokenType::NIL))
    {
   
        return Factory::Instance().createIntegerLiteral(0);
   
    }
    if (match(TokenType::STRING))
    {
        std::string value = previous().literal;
     
        return Factory::Instance().createStringLiteral(value);
    }
    
    if (match(TokenType::FLOAT))
    {
        double value =std::stod(previous().literal);
       
        return Factory::Instance().createFloatLiteral(value);
    }

    if (match(TokenType::INT))
    {
        int value = std::stoi(previous().literal);
       
        return Factory::Instance().createIntegerLiteral(value);
    }

    if (match(TokenType::BYTE))
    {
        int value = std::stoi(previous().literal);
       
        return Factory::Instance().createByteLiteral(value);
    }

  
  

    if (match(TokenType::IDENTIFIER))
    {
         std::shared_ptr<VariableExpr> expr = std::make_shared<VariableExpr>(previous());
        if (match(TokenType::INC))
        {
            Token op = previous();
            return std::make_shared<UnaryExpr>(expr, op,false);
        } else  if (match(TokenType::DEC))
        {
            Token op = previous();
            return std::make_shared<UnaryExpr>(expr, op,false);
         } 
        //else if (match(TokenType::LEFT_PAREN) )
        //  {
        //      return nativeFunctionCall(expr);
        //  }

        return expr;
    }


    if (match(TokenType::LEFT_PAREN))
    {
        std::shared_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN,"Expect ')' after expression.");
        return std::make_shared<GroupingExpr>(expr);
    }
    
    return std::make_shared<EmptyExpr>();

    
}

//******************************************* */

std::shared_ptr<Stmt> Parser::statement()
{
    
    if (match(TokenType::PRINT))
    {
        return printStmt();
    }
    if (match(TokenType::BEGIN))
    {
        
        return blockStmt();
    }
    if (match(TokenType::IDPROCEDURE))
    {
        return procedureCall();
    }
 
    if (match(TokenType::SWITCH))
    {
        return switchStmt();
    }
    if (match(TokenType::REPEAT))
    {
        return repeatStmt();
    }

    if (match(TokenType::FOR))
    {
        return forStmt();
    }

    if (match(TokenType::BREAK))
    {
        return breakStmt();
    }

    if (match(TokenType::CONTINUE))
    {
        return continueStmt();
    }


    if (match(TokenType::LOOP))
    {
        return loopStmt();
    }

    if (match(TokenType::IF))
    {
        return ifStmt();
    }

    if (match(TokenType::WHILE))
    {
        return whileStmt();
    }

    if (match(TokenType::RETURN))
    {
        return returnStmt();
    }
   


    return expressionStatement();
}

std::shared_ptr<ReturnStmt> Parser::returnStmt()
{
    std::shared_ptr<Expr> exp = expression();
    consume(TokenType::SEMICOLON,"Expect ';' after expression.");
    return std::make_shared<ReturnStmt>(std::move(exp));
}
std::shared_ptr<IfStmt> Parser::ifStmt()
{
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
    std::shared_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
    std::shared_ptr<Stmt> thenBranch = statement();
    
    std::vector<std::unique_ptr<ElifStmt>> elifBranch;
    while (match(TokenType::ELIF))
    {
        consume(TokenType::LEFT_PAREN, "Expect '(' after 'elif'.");
        std::shared_ptr<Expr> condition = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
        std::shared_ptr<Stmt> thenBranch = statement();
        elifBranch.push_back(std::make_unique<ElifStmt>(std::move(condition), std::move(thenBranch)));
    }
    
    std::shared_ptr<Stmt> elseBranch = nullptr;
    if (match(TokenType::ELSE))
    {
        elseBranch = statement();
    }
    
    return std::make_shared<IfStmt>(std::move(condition), std::move(thenBranch), std::move(elseBranch), std::move(elifBranch));
}

std::shared_ptr<ForStmt> Parser::forStmt()
{
    
     

    consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");

    std::shared_ptr<Stmt> initializer = nullptr;
    if (match(TokenType::SEMICOLON))
    {
        Error(peek(), "Missing 'for' initializer.");
    }
    else  if (match(TokenType::IDINT))
    {
        initializer = varDeclaration(LiteralType::INT);
    }
    else if (match(TokenType::IDFLOAT))
    {
        initializer = varDeclaration(LiteralType::FLOAT);
    }
    else if (match(TokenType::IDBYTE))
    {
        initializer = varDeclaration(LiteralType::BYTE);
    } else
    {
         initializer = expressionStatement();
    }
    if (match(TokenType::SEMICOLON))
    {
        Error(peek(), "Missing 'for' condition.");
    }
    std::shared_ptr<Expr> condition = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after condition.");

    if (match(TokenType::SEMICOLON))
    {
        Error(peek(), "Missing 'for' step.");
    }
    std::shared_ptr<Expr> step = expression();

    consume(TokenType::RIGHT_PAREN, "Expect ')' after for step.");

    std::shared_ptr<Stmt> body = statement();

    return std::make_shared<ForStmt>(std::move(initializer), std::move(condition), std::move(step), std::move(body));
}

std::shared_ptr<WhileStmt> Parser::whileStmt()
{
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
    std::shared_ptr<Expr> condition = expression(); 
    consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
    std::shared_ptr<Stmt> body = statement();

    return std::make_shared<WhileStmt>(std::move(condition), std::move(body));

}

std::shared_ptr<PrintStmt> Parser::printStmt()
{
    consume(TokenType::LEFT_PAREN,"Expect '(' after 'print'.");
    std::shared_ptr<Expr> exp = expression();
    consume(TokenType::RIGHT_PAREN,"Expect ')' after value.");
    consume(TokenType::SEMICOLON,"Expect ';' after value.");
    return std::make_shared<PrintStmt>(std::move(exp));
}

std::shared_ptr<BlockStmt> Parser::blockStmt()
{
    
    std::vector<std::shared_ptr<Stmt>> statements;

     while (!check(TokenType::END) && !isAtEnd()) 
    {
    
        statements.push_back(declaration());
    }
    consume(TokenType::END,"Expect 'end' after block.");
    match(TokenType::SEMICOLON);
    return std::make_shared<BlockStmt>(std::move(statements));
}

std::shared_ptr<Program> Parser::programStmt()
{
    consume(TokenType::PROGRAM,"Expect 'program' at the beginning.");
    Token name = consume(TokenType::IDENTIFIER,"Expect program name after 'program'.");
    std::string nameStr = name.literal;
    consume(TokenType::SEMICOLON,"Expect ';' after program name.");

    std::vector<std::shared_ptr<Stmt>> statements;
    while (!check(TokenType::BEGIN) && !isAtEnd())
     {
    //     std::cout<<" block "<<peek().literal<<std::endl;
         statements.push_back(defDeclaration());
     }
   
    
    std::shared_ptr<Stmt> block = statement();
    consume(TokenType::DOT,"Expect '.' after end of program block.");

    return std::make_shared<Program>(nameStr, std::move(statements), std::move(block));
    
}

std::shared_ptr<SwitchStmt> Parser::switchStmt()
{

    consume(TokenType::LEFT_PAREN,"Expect '(' after 'switch'.");
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::RIGHT_PAREN,"Expect ')' after condition.");
    consume(TokenType::BEGIN,"Expect 'begin' after condition.");
    
    
    std::vector<std::unique_ptr<CaseStmt>> cases;
    while (match(TokenType::CASE))
    {
        std::shared_ptr<Expr> exp = expression();
        consume(TokenType::COLON,"Expect ':' after case expression.");
        std::shared_ptr<Stmt> body = statement();
        cases.push_back(std::make_unique<CaseStmt>(std::move(exp), std::move(body)));
    }
    std::shared_ptr<Stmt> default_case= nullptr;
    if (match(TokenType::DEFAULT))
    {
        consume(TokenType::COLON,"Expect ':' after default case.");
        default_case = statement();
    }

    consume(TokenType::END,"Expect 'end' after switch block.");
   
    return std::make_shared<SwitchStmt>(std::move(expr), std::move(default_case), std::move(cases));

}

std::shared_ptr<RepeatStmt> Parser::repeatStmt()
{
    std::shared_ptr<Stmt> body = statement();
 
    consume(TokenType::UNTIL,"Expect 'until' after repeat block.");
    consume(TokenType::LEFT_PAREN,"Expect '(' after 'until'.");
    std::shared_ptr<Expr> exp = expression();
    
    consume(TokenType::RIGHT_PAREN,"Expect ')' after condition.");
    return std::make_shared<RepeatStmt>(std::move(exp), std::move(body));
    
}

std::shared_ptr<LoopStmt> Parser::loopStmt()
{
        match(TokenType::SEMICOLON);
        std::shared_ptr<Stmt> body = statement();
        return std::make_shared<LoopStmt>(std::move(body));
}

std::shared_ptr<BreakStmt> Parser::breakStmt()
{

    return std::make_shared<BreakStmt>();
}

std::shared_ptr<ContinueStmt> Parser::continueStmt()
{
    return std::make_shared<ContinueStmt>();
}

std::shared_ptr<FunctionStmt> Parser::functionStmt()
{

    Token name = consume(TokenType::IDFUNCTION,"Expect function name .");
    std::string nameStr = name.literal;
    consume(TokenType::LEFT_PAREN,"Expect '(' after function name.");
    std::vector<std::shared_ptr<Argument>> parameter;
    if (!check(TokenType::RIGHT_PAREN) || !isAtEnd())
    {
        do
        {
            
           
            if (match(TokenType::IDINT))
            {
                    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
                    
                    std::shared_ptr<LiteralExpr> expr = Factory::Instance().Instance().createIntegerLiteral(DEFAULT_INT);
                    std::shared_ptr<Argument> arg = std::make_shared<Argument>(name.literal, std::move(expr));
                    parameter.push_back(arg);

            } else 
            if (match(TokenType::IDFLOAT))
            {
                    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
                    std::shared_ptr<LiteralExpr> expr = Factory::Instance().Instance().createFloatLiteral(DEFAULT_FLOAT);
                    std::shared_ptr<Argument> arg = std::make_shared<Argument>(name.literal, std::move(expr));
                    parameter.push_back(arg);
            } else
            if (match(TokenType::IDBYTE))
            {
                    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
                    std::shared_ptr<LiteralExpr> expr = Factory::Instance().Instance().createByteLiteral(DEFAULT_BYTE);
                    std::shared_ptr<Argument> arg = std::make_shared<Argument>(name.literal, std::move(expr));
                    parameter.push_back(arg);
            } else
            if (match(TokenType::IDSTRING))
            {
                Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
                std::shared_ptr<LiteralExpr> expr = Factory::Instance().Instance().createStringLiteral(DEFAULT_STRING);
                std::shared_ptr<Argument> arg = std::make_shared<Argument>(name.literal, std::move(expr));
                parameter.push_back(arg);
               
            } else 
            if (match(TokenType::IDBOOL))
            {
                    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
                    std::shared_ptr<LiteralExpr> expr = Factory::Instance().Instance().createBoolLiteral(false);
                    std::shared_ptr<Argument> arg = std::make_shared<Argument>(name.literal, std::move(expr));
                    parameter.push_back(arg);
            }
        } while (match(TokenType::COMMA));
    } 

    LiteralType returnType;

    consume(TokenType::RIGHT_PAREN,"Expect ')' after function arguments."); 
    consume(TokenType::COLON,"Expect ':' after function arguments."); 

     
    if (match(TokenType::IDINT))
    {
        returnType = LiteralType::INT;
    } else 
    if (match(TokenType::IDFLOAT))
    {
        returnType = LiteralType::FLOAT;
    } else 
    if (match(TokenType::IDBYTE))
    {
        returnType = LiteralType::BYTE;
    } else
    if (match(TokenType::IDSTRING))
    {
        returnType = LiteralType::STRING;
    } else 
    if (match(TokenType::IDBOOL))
    {
        returnType = LiteralType::BOOLEAN;
    } else 
    {
        Error(tokens[current],"Expect function return type .");
        
    }
    
 

    if (!check(TokenType::BEGIN))
    {
        Error(tokens[current],"Expect 'begin'  ");
    }



    std::shared_ptr<Stmt> body = statement();
    return std::make_shared<FunctionStmt>(nameStr, returnType, std::move(parameter), std::move(body));
}



std::shared_ptr<ProcedureStmt> Parser::procedureStmt()
{
    
    Token name = consume(TokenType::IDPROCEDURE,"Expect procedure name .");
    std::string nameStr = name.literal;
    consume(TokenType::LEFT_PAREN,"Expect '(' after procedure name.");
    std::vector<std::shared_ptr<Argument>> parameter;
    if (!check(TokenType::RIGHT_PAREN) || !isAtEnd())
    {
        do
        {
            
          
            if (match(TokenType::IDINT))
            {
                    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
                    
                    std::shared_ptr<LiteralExpr> expr = Factory::Instance().Instance().createIntegerLiteral(DEFAULT_INT);
                    std::shared_ptr<Argument> arg = std::make_shared<Argument>(name.literal, std::move(expr));
                    parameter.push_back(arg);

            } else if (match(TokenType::IDFLOAT))
            {
                    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
                    std::shared_ptr<LiteralExpr> expr = Factory::Instance().Instance().createFloatLiteral(DEFAULT_FLOAT);
                    std::shared_ptr<Argument> arg = std::make_shared<Argument>(name.literal, std::move(expr));
                    parameter.push_back(arg);
            }  else if (match(TokenType::IDBYTE))
            {
                    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
                    std::shared_ptr<LiteralExpr> expr = Factory::Instance().Instance().createByteLiteral(DEFAULT_BYTE);
                    std::shared_ptr<Argument> arg = std::make_shared<Argument>(name.literal, std::move(expr));
                    parameter.push_back(arg);
            }else 
            if (match(TokenType::IDSTRING))
            {
                Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
                std::shared_ptr<LiteralExpr> expr = Factory::Instance().Instance().createStringLiteral(DEFAULT_STRING);
                std::shared_ptr<Argument> arg = std::make_shared<Argument>(name.literal, std::move(expr));
                parameter.push_back(arg);
               
            } else 
            if (match(TokenType::IDBOOL))
            {
                    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
                    std::shared_ptr<LiteralExpr> expr = Factory::Instance().Instance().createBoolLiteral(false);
                    std::shared_ptr<Argument> arg = std::make_shared<Argument>(name.literal, std::move(expr));
                    parameter.push_back(arg);
            }
        } while (match(TokenType::COMMA));
    }
    
    consume(TokenType::RIGHT_PAREN,"Expect ')' after procedure name.");
    if (!check(TokenType::BEGIN))
    {
        Error(tokens[current],"Expect 'begin'  ");
    }

    std::shared_ptr<Stmt> body = statement();
    return std::make_shared<ProcedureStmt>(nameStr, std::move(parameter), std::move(body));
}


std::shared_ptr<ProcedureCallStmt> Parser::procedureCall()
{

    std::vector<std::shared_ptr<Expr>> arguments;
    Token name = previous();
    consume(TokenType::LEFT_PAREN, "Expect '(' after procedure name.");
     
    if (match(TokenType::RIGHT_PAREN))
    {
     
    }else 
    do
    {   
            std::shared_ptr<Expr> value = expression();
            arguments.push_back(value);  
            if (match(TokenType::RIGHT_PAREN)) break;
            consume(TokenType::COMMA, "Expect ',' after arguments.");
            
    }while (!check(TokenType::RIGHT_PAREN) || !isAtEnd());
    consume(TokenType::SEMICOLON, "Expect ';' after procedure arguments.");
    return std::make_shared<ProcedureCallStmt>(name, std::move(arguments));
}

std::shared_ptr<CallerExpr> Parser::processCall()
{

   
    std::vector<std::shared_ptr<Expr>> arguments;
    Token token = previous();
    
    consume(TokenType::LEFT_PAREN, "Expect '(' after process name.");

    std::string name = token.lexeme;
    int line = token.line;
   
     
    if (match(TokenType::RIGHT_PAREN))
    {
     
    }else 
    do
    {   
            std::shared_ptr<Expr> value = expression();
            arguments.push_back(value);
            if (match(TokenType::RIGHT_PAREN)) break;
            consume(TokenType::COMMA, "Expect ',' after arguments.");
            
    }while (!check(TokenType::RIGHT_PAREN) || !isAtEnd());

    unsigned int arity = arguments.size();
    return std::make_shared<CallerExpr>(name,line, std::move(arguments), arity,0);
}


std::shared_ptr<CallerExpr> Parser::nativeFunctionCall()
{

    std::vector<std::shared_ptr<Expr>> arguments;
    Token token = previous();
        std::string name = token.lexeme;
    int line = token.line;
   
   
    consume(TokenType::LEFT_PAREN, "Expect '(' after funciton name.");
    
     
    if (match(TokenType::RIGHT_PAREN))
    {
     
    }else 
    do
    {   
          
            std::shared_ptr<Expr> value = expression();
            arguments.push_back(value);  
            if (match(TokenType::RIGHT_PAREN)) break;
            consume(TokenType::COMMA, "Expect ',' after arguments.");
            
    }while (!check(TokenType::RIGHT_PAREN) || !isAtEnd());

    unsigned int arity = arguments.size();
    return std::make_shared<CallerExpr>(name,line, std::move(arguments), arity,2);
}

std::shared_ptr<CallerExpr> Parser::functionCall()
{

    std::vector<std::shared_ptr<Expr>> arguments;

    Token token = previous();
   
    std::string name = token.lexeme;
    int line = token.line;
    consume(TokenType::LEFT_PAREN, "Expect '(' after function name.");
   

  //  Log(1,"function call %s : %s : %s",peek().toString().c_str(),previous().toString().c_str(),lookAhead().toString().c_str());
        

    if (match(TokenType::RIGHT_PAREN))
    {
     
    }else 
    do
    {   
            std::shared_ptr<Expr> value = expression();
            arguments.push_back(value);
            if (match(TokenType::RIGHT_PAREN)) break;
            consume(TokenType::COMMA, "Expect ',' after arguments.");
            
    }while (!check(TokenType::RIGHT_PAREN) || !isAtEnd());

    unsigned int arity = arguments.size();
    
    return std::make_shared<CallerExpr>(name,line, std::move(arguments), arity,1);
  
}

std::shared_ptr<ProcessStmt> Parser::processStmt()
{
    Token name = consume(TokenType::IDPROCESS,"Expect process name .");
    std::string nameStr = name.literal;
    consume(TokenType::LEFT_PAREN,"Expect '(' after process name.");
    std::vector<std::shared_ptr<Argument>> parameter;
    if (!check(TokenType::RIGHT_PAREN) || !isAtEnd())
    {
        do
        {
            
            
            if (match(TokenType::IDINT))
            {
                    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
                    
                    std::shared_ptr<LiteralExpr> expr = Factory::Instance().Instance().createIntegerLiteral(DEFAULT_INT);
                    std::shared_ptr<Argument> arg = std::make_shared<Argument>(name.literal, std::move(expr));
                    parameter.push_back(arg);

            } else if (match(TokenType::IDFLOAT))
            {
                    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
                    std::shared_ptr<LiteralExpr> expr = Factory::Instance().Instance().createFloatLiteral(DEFAULT_FLOAT);
                    std::shared_ptr<Argument> arg = std::make_shared<Argument>(name.literal, std::move(expr));
                    parameter.push_back(arg);
            }else 
            if (match(TokenType::IDBYTE))
            {
                    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
                    std::shared_ptr<LiteralExpr> expr = Factory::Instance().Instance().createByteLiteral(DEFAULT_BYTE);
                    std::shared_ptr<Argument> arg = std::make_shared<Argument>(name.literal, std::move(expr));
                    parameter.push_back(arg);
            }else
            if (match(TokenType::IDSTRING))
            {
                Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
                std::shared_ptr<LiteralExpr> expr = Factory::Instance().Instance().createStringLiteral(DEFAULT_STRING);
                std::shared_ptr<Argument> arg = std::make_shared<Argument>(name.literal, std::move(expr));
                parameter.push_back(arg);
               
            } else 
            if (match(TokenType::IDBOOL))
            {
                    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
                    std::shared_ptr<LiteralExpr> expr = Factory::Instance().Instance().createBoolLiteral(false);
                    std::shared_ptr<Argument> arg = std::make_shared<Argument>(name.literal, std::move(expr));
                    parameter.push_back(arg);
            }
        } while (match(TokenType::COMMA));
    }
    
    consume(TokenType::RIGHT_PAREN,"Expect ')' after process name.");
    if (!check(TokenType::BEGIN))
    {
        Error(tokens[current],"Expect 'begin'  ");
    }

    std::shared_ptr<Stmt> body = statement();
    return std::make_shared<ProcessStmt>(nameStr, std::move(parameter), std::move(body));
}





std::shared_ptr<ExpressionStmt> Parser::expressionStatement()
{
     
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON,"Expect ';' after expression statement.");
    return std::make_shared<ExpressionStmt>(std::move(expr));
}

std::shared_ptr<EmptyStmt> Parser::emptyDeclaration()
{
    return std::make_shared<EmptyStmt>();
}

std::shared_ptr<Stmt> Parser::declaration()
{

   
    if (match(TokenType::IDINT))
    {
        return varDeclaration(LiteralType::INT);
    } else 
    if (match(TokenType::IDFLOAT))
    {
        return varDeclaration(LiteralType::FLOAT);
    } else 
    if (match(TokenType::IDBYTE))
    {
        return varDeclaration(LiteralType::BYTE);
    }else 
    if (match(TokenType::IDSTRING))
    {
        return varDeclaration(LiteralType::STRING);
    } else 
    if (match(TokenType::IDBOOL))
    {
        return varDeclaration(LiteralType::BOOLEAN);
    }
    return statement();
}

std::shared_ptr<Stmt> Parser::defDeclaration()
{

    
    if (match(TokenType::FUNCTION))
    {
        return functionStmt();   
    }  
    if (match(TokenType::PROCEDURE))
    {
       return procedureStmt();
    }
    if (match(TokenType::PROCESS))
    {
       return processStmt();
    } 
    return emptyDeclaration();
}



std::shared_ptr<VarStmt> Parser::varDeclaration(LiteralType type)
{
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");

    std::vector<Token> names;
    names.push_back(name);

    std::shared_ptr<Expr> initializer = nullptr;


    if (match(TokenType::EQUAL))
    {
        initializer = expression();
    } else 
    {
        while (match(TokenType::COMMA) && !isAtEnd())
        {
           Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
           names.push_back(name);
        }
        if (match(TokenType::EQUAL))
        {

            initializer = expression();
        } else 
        { 

            if (type == LiteralType::INT)
            {
                initializer = Factory::Instance().Instance().createIntegerLiteral(DEFAULT_INT);
            } else 
            if (type == LiteralType::FLOAT)
            {
                initializer = Factory::Instance().Instance().createFloatLiteral(DEFAULT_FLOAT);
            } else 
            if (type == LiteralType::BYTE)
            {
                initializer = Factory::Instance().Instance().createByteLiteral(DEFAULT_BYTE);
            } else
            if (type == LiteralType::STRING)
            {
                initializer =Factory::Instance().Instance().createStringLiteral(DEFAULT_STRING);
            } else 
            if (type == LiteralType::BOOLEAN)
            {
                initializer =Factory::Instance().Instance().createBoolLiteral(false);
            } else 
            {
                Warning(name,"Type not supported for variable assign");
            }
        }

    }
    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_shared<VarStmt>(names, std::move(initializer), type);
}



