#include "pch.h"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"



    
Parser::Parser(const std::vector<Token> &tokens)
{
this->tokens = tokens;
current = 0;
panicMode = false;
countBegins = 0;
countEnds = 0;
}



std::unique_ptr<Expr> Parser::result()
{

    return   logic_or();
       

}

std::unique_ptr<Stmt> Parser::parse()
{
    try
    {
        return programStmt();
    }
    catch(const std::exception& e)
    {
        synchronize();
        std::cerr << e.what() << '\n';
        return nullptr;
    }
    
   
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
    if (current + 1 >= tokens.size()) return previous();
    return tokens[current + 1];
}



void Parser::Error(const Token &token,const std::string &message)
{
    panicMode = true;
    int line = token.line;
    std::string text ="ERROR: " +message+ " at line: " +std::to_string(line);
    throw std::runtime_error(text);
}

void Parser::Warning(const Token &token,const std::string &message)
{
    int line = token.line;
    std::string text ="WARNING: " +message+ " at line: " +std::to_string(line);
    throw std::runtime_error(text);
}

std::unique_ptr<Expr> Parser::expression()
{
    return  assignment();
}

std::unique_ptr<Expr> Parser::assignment()
{
    std::unique_ptr<Expr> expr = logic_or();

    // std::cout<<"Assign: "<< expr->toString()<<" type: "<<peek().toString() << std::endl;
    Token name = previous();
    if (match(TokenType::EQUAL))
    {
        std::unique_ptr<Expr> value = assignment();
     
      //  std::cout<<"Assign: "<<name.lexeme<< " "<< expr->toString()+" = "<<value->toString() << std::endl;

         if (expr->getType() == ExprType::VARIABLE)
         {
            return  std::make_unique<AssignExpr>(name, std::move(value));
         }

    }
    return expr;
}


std::unique_ptr<Expr> Parser::logic_or()
{
    std::unique_ptr<Expr> expr = logic_and();

    while (match(TokenType::OR))
    {
        Token op = previous();
        std::unique_ptr<Expr> right = logic_and();
        expr = std::make_unique<LogicalExpr>(std::move(expr), std::move(right), op);
    }
    return expr;
}

std::unique_ptr<Expr> Parser::logic_and()
{
    std::unique_ptr<Expr> expr = logic_xor();

    while (match(TokenType::AND))
    {
        Token op = previous();
        std::unique_ptr<Expr> right = logic_xor();
        expr = std::make_unique<LogicalExpr>(std::move(expr), std::move(right), op);
    }
    return expr;
}

std::unique_ptr<Expr> Parser::logic_xor()
{
    std::unique_ptr<Expr> expr = equality();

    while (match(TokenType::XOR))
    {
        Token op = previous();
        std::unique_ptr<Expr> right = equality();
        expr = std::make_unique<LogicalExpr>(std::move(expr), std::move(right), op);
    }
    return expr;
}


std::unique_ptr<Expr> Parser::equality()
{
    std::unique_ptr<Expr> expr = comparison();
    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL}))
    {
        Token op = previous();
        std::unique_ptr<Expr> right = comparison();
        expr = std::make_unique<BinaryExpr>(std::move(expr), std::move(right), op);
    }
    return expr;
}

std::unique_ptr<Expr> Parser::comparison()
{
    std::unique_ptr<Expr> expr = term();
    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL}))
    {
        Token op = previous();
        std::unique_ptr<Expr> right = term();
        expr = std::make_unique<BinaryExpr>(std::move(expr), std::move(right), op);
    }
    return expr;
}



std::unique_ptr<Expr> Parser::term()
{
    std::unique_ptr<Expr> expr = factor();
    while (match({TokenType::MINUS, TokenType::PLUS}))
    {
        Token op = previous();
        std::unique_ptr<Expr> right = factor();
        expr = std::make_unique<BinaryExpr>(std::move(expr), std::move(right), op);
    }
    return expr;
}

std::unique_ptr<Expr> Parser::factor()
{
    std::unique_ptr<Expr> expr = power();//^
    while (match({TokenType::STAR, TokenType::SLASH, TokenType::MOD}))
    {
        Token op = previous();
        std::unique_ptr<Expr> right = power();//^
        expr = std::make_unique<BinaryExpr>(std::move(expr), std::move(right), op);
    }
    return expr;
}

std::unique_ptr<Expr> Parser::power()
{
    std::unique_ptr<Expr> expr = unary();
    while (match(TokenType::POWER))
    {
        Token op = previous();
        std::unique_ptr<Expr> right = unary();
        expr = std::make_unique<BinaryExpr>(std::move(expr), std::move(right), op);
    }
    return expr;
}

std::unique_ptr<Expr> Parser::unary()
{
    if (match({TokenType::BANG, TokenType::MINUS, TokenType::NOT}))
    {
        Token op = previous();
        std::unique_ptr<Expr> right = unary();
        return std::make_unique<UnaryExpr>(std::move(right), op);
    }
    return primary();
}

std::unique_ptr<Stmt> Parser::call_stmt(const std::string &name)
{

    // std::vector<std::unique_ptr<LiteralExpr>> arguments;
    // //std::unique_ptr<Expr> expCurrent = expression();

    
    // // if (!check(TokenType::RIGHT_PAREN) || !isAtEnd())
    // // {
    // //    // arguments.push_back(std::move(expCurrent));
    // //     consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");
    // //     consume(TokenType::SEMICOLON, "Expect ';' after arguments.");
    // //     return std::make_unique<CallStmt>(name, std::move( arguments));
    // // }
     

    // while (!isAtEnd())
    // {   

            

    //         std::unique_ptr<Expr> value = primary();
    //         LiteralExpr *literal = dynamic_cast<LiteralExpr*>(value.get());

    //         std::cout<<"Call: "<< " "<<literal->toString()<<std::endl;
           
    //         if (check(TokenType::RIGHT_PAREN))
    //         {
    //             std::cout<<"end Call: "<< name <<" "<<peek().literal<<std::endl;
    //             break;
    //         }

    //         consume(TokenType::COMMA, "Expect ',' after arguments.");

    //         arguments.push_back(std::move(exp));      

            
      
    // }

    // consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");
    // consume(TokenType::SEMICOLON, "Expect ';' after arguments.");


   

    // return std::make_unique<CallStmt>(name, std::move( arguments));
    return std::make_unique<EmptyStmt>();
}

std::unique_ptr<Expr> Parser::call()
{
   
    // std::unique_ptr<Expr> expr = primary();
    // while (true)
    // {
        
    //      std::cout<<"call: "<<expr->toString()<<std::endl;
    //     if (match(TokenType::LEFT_PAREN))
    //     {
    //         std::cout<<"call"<<std::endl;
    //        // expr = finishCall(expr);
    //          std::vector<std::unique_ptr<LiteralExpr>> arguments;

    //         if (!check(TokenType::RIGHT_PAREN)) 
    //         {
    //             do {

    //              expression();
    //              // arguments.push_back(str::move(expression()));

    //             } while (match(TokenType::COMMA));
    //         }

    //         consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");

    //     }
    //     else
    //     {
    //         break;
    //     }
      
    // }

  return std::make_unique<EmptyExpr>();
}


std::unique_ptr<Expr> Parser::primary()
{
    if (match(TokenType::NOW))
    {
       return std::make_unique<NowExpr>();
    }

  

    if (match(TokenType::IDENTIFIER))
    {
        return std::make_unique<VariableExpr>(previous());
    }


 

    if (match(TokenType::FALSE))
    {
          auto literal =std::make_unique<Literal>(false);
          return std::make_unique<LiteralExpr>(std::move(literal));
    }
    if (match(TokenType::TRUE))
    {
           auto literal =std::make_unique<Literal>(true);
          return std::make_unique<LiteralExpr>(std::move(literal));
   
    }
    if (match(TokenType::NIL))
    {
        auto literal =std::make_unique<Literal>(nullptr);
        return std::make_unique<LiteralExpr>(std::move(literal));
   
    }
    if (match(TokenType::STRING))
    {
        std::string value = previous().literal;
        std::unique_ptr<Literal> literal = std::make_unique<Literal>(value);
        return std::make_unique<LiteralExpr>(std::move(literal));
    }
    if (match(TokenType::INT))
    {
        int value =std::stoi(previous().literal);
        std::unique_ptr<Literal> literal = std::make_unique<Literal>(value);
        return std::make_unique<LiteralExpr>(std::move(literal));
    }
    if (match(TokenType::FLOAT))
    {
        float value =std::stof(previous().literal);
        std::unique_ptr<Literal> literal = std::make_unique<Literal>(value);
        return std::make_unique<LiteralExpr>(std::move(literal));
    }
    if (match(TokenType::LEFT_PAREN))
    {
        std::unique_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN,"Expect ')' after expression.");
        return std::make_unique<GroupingExpr>(std::move(expr));
    }
    
    return std::make_unique<EmptyExpr>();

    
}

//******************************************* */

std::unique_ptr<Stmt> Parser::statement()
{
    if (match(TokenType::PRINT))
    {
        return printStmt();
    }
    if (match(TokenType::BEGIN))
    {
        countBegins++;
        return blockStmt();
    }
    if (match(TokenType::IDPROCEDURE))
    {
        return procedureCall();
    }


    return expressionStatement();
}

std::unique_ptr<PrintStmt> Parser::printStmt()
{
    consume(TokenType::LEFT_PAREN,"Expect '(' after 'print'.");
    std::unique_ptr<Expr> exp = expression();
    consume(TokenType::RIGHT_PAREN,"Expect ')' after value.");
    consume(TokenType::SEMICOLON,"Expect ';' after value.");
    return std::make_unique<PrintStmt>(std::move(exp));
}

std::unique_ptr<BlockStmt> Parser::blockStmt()
{
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!check(TokenType::END) && !isAtEnd())
    {
        
        statements.push_back(declaration());
    }
    consume(TokenType::END,"Expect 'end' after block.");
    return std::make_unique<BlockStmt>(std::move(statements));
}

std::unique_ptr<Program> Parser::programStmt()
{
    consume(TokenType::PROGRAM,"Expect 'program' at the beginning.");
    Token name = consume(TokenType::IDENTIFIER,"Expect program name after 'program'.");
    std::string nameStr = name.literal;
    consume(TokenType::SEMICOLON,"Expect ';' after program name.");

    std::vector<std::unique_ptr<Stmt>> statements;
    while (!check(TokenType::BEGIN) && !isAtEnd())
     {
    //     std::cout<<" block "<<peek().literal<<std::endl;
         statements.push_back(defDeclaration());
     }

    //  while (currentToken() != "begin") {
    //     if (currentToken() == "function") {
    //         program.functions.push_back(parseFunction());
    //     } else {
    //         // Erro: esperava função ou begin
    //     }
    // }

   
   
    
    std::unique_ptr<Stmt> block = statement();
    consume(TokenType::DOT,"Expect '.' after end of program block.");

    return std::make_unique<Program>(nameStr, std::move(statements), std::move(block));
    
}

std::unique_ptr<ProcedureStmt> Parser::procedureStmt()
{
    
    Token name = consume(TokenType::IDPROCEDURE,"Expect procedure name .");
    std::string nameStr = name.literal;
    consume(TokenType::LEFT_PAREN,"Expect '(' after procedure name.");
    std::vector<std::unique_ptr<Argument>> parameter;
    if (!check(TokenType::RIGHT_PAREN) || !isAtEnd())
    {
        do
        {
            
            if (match(TokenType::IDINT))
            {
                  Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
                  std::unique_ptr<LiteralExpr> expr = Interpreter::createIntLiteral(INT32_MAX);
                  std::unique_ptr<Argument> arg = std::make_unique<Argument>(name.literal, std::move(expr));
                  parameter.push_back(std::move(arg));
            } else 
            if (match(TokenType::IDFLOAT))
            {
                    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
                    
                    std::unique_ptr<LiteralExpr> expr = Interpreter::createFloatLiteral(MAXFLOAT);
                    std::unique_ptr<Argument> arg = std::make_unique<Argument>(name.literal, std::move(expr));
                    parameter.push_back(std::move(arg));

            } else 
            if (match(TokenType::IDSTRING))
            {
                Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
                std::unique_ptr<LiteralExpr> expr = Interpreter::createStringLiteral("");
                std::unique_ptr<Argument> arg = std::make_unique<Argument>(name.literal, std::move(expr));
                parameter.push_back(std::move(arg));
               
            } else 
            if (match(TokenType::IDBOOL))
            {
                    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
                    std::unique_ptr<LiteralExpr> expr = Interpreter::createBoolLiteral(false);
                    std::unique_ptr<Argument> arg = std::make_unique<Argument>(name.literal, std::move(expr));
                    parameter.push_back(std::move(arg));
            }
        } while (match(TokenType::COMMA));
    }
    
    consume(TokenType::RIGHT_PAREN,"Expect ')' after procedure name.");
    if (!check(TokenType::BEGIN))
    {
        Error(tokens[current],"Expect 'begin'  ");
    }

    std::unique_ptr<Stmt> body = statement();
    return std::make_unique<ProcedureStmt>(nameStr, std::move(parameter), std::move(body));
}

std::unique_ptr<ProcedureCallStmt> Parser::procedureCall()
{

    std::vector<std::unique_ptr<Expr>> arguments;
    Token name = previous();
    consume(TokenType::LEFT_PAREN, "Expect '(' after procedure name.");
     
    if (match(TokenType::RIGHT_PAREN))
    {
     
    }else 
    do
    {   
          
            std::unique_ptr<Expr> value = primary();
           // if (value->getType() != ExprType::EMPTY_EXPR)
            arguments.push_back(std::move(value));  
            if (match(TokenType::RIGHT_PAREN)) break;
            consume(TokenType::COMMA, "Expect ',' after arguments.");
            
    }while (!check(TokenType::RIGHT_PAREN) || !isAtEnd());
    
    


    consume(TokenType::SEMICOLON, "Expect ';' after procedure arguments.");

    return std::make_unique<ProcedureCallStmt>(name, std::move(arguments));
}

std::unique_ptr<ExpressionStmt> Parser::expressionStatement()
{
     
    std::unique_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON,"Expect ';' after expression statement.");
    return std::make_unique<ExpressionStmt>(std::move(expr));
}

std::unique_ptr<EmptyStmt> Parser::emptyDeclaration()
{
    return std::make_unique<EmptyStmt>();
}

std::unique_ptr<Stmt> Parser::declaration()
{

    if (match(TokenType::IDINT))
    {
        return varDeclaration(LiteralType::INT);
    } else 
    if (match(TokenType::IDFLOAT))
    {
        return varDeclaration(LiteralType::FLOAT);
    } else 
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

std::unique_ptr<Stmt> Parser::defDeclaration()
{

   
    
    if (match(TokenType::FUNCTION))
    {
        
    }  
    if (match(TokenType::PROCEDURE))
    {
       return procedureStmt();
    }
    if (match(TokenType::PROCESS))
    {
       
    } 
    return emptyDeclaration();
}

std::unique_ptr<VarStmt> Parser::varDeclaration(LiteralType type)
{
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");

    std::vector<Token> names;
    names.push_back(name);

    std::unique_ptr<Expr> initializer;


    if (match(TokenType::EQUAL))
    {
        initializer = expression();
    } else 
    {
        while (match(TokenType::COMMA) && !isAtEnd())
        {
           Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
           names.push_back(name);
           std::cout<<name.literal<<std::endl;
        }
        if (match(TokenType::EQUAL))
        {
            initializer = expression();
        } else 
        { 

            if (type == LiteralType::INT)
            {
                initializer = std::make_unique<LiteralExpr>(std::make_unique<Literal>(INT32_MAX));
            } else 
            if (type == LiteralType::FLOAT)
            {
                initializer = std::make_unique<LiteralExpr>(std::make_unique<Literal>(MAXFLOAT));
            } else 
            if (type == LiteralType::STRING)
            {
                initializer = std::make_unique<LiteralExpr>(std::make_unique<Literal>(""));
            } else 
            if (type == LiteralType::BOOLEAN)
            {
                initializer = std::make_unique<LiteralExpr>(std::make_unique<Literal>(false));
            } else 
            if (type == LiteralType::POINTER)
            {
                initializer = std::make_unique<LiteralExpr>(std::make_unique<Literal>(nullptr));
            } else
            {
                Warning(name,"Type not supported for variable assign");
            }
        }

    }
    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_unique<VarStmt>(std::move(names), std::move(initializer));
}


