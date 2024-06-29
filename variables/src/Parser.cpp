#include "pch.h"
#include "Lexer.hpp"
#include "Parser.hpp"




    
Parser::Parser(const std::vector<Token> &tokens)
{
this->tokens = tokens;
current = 0;
panicMode = false;
}

bool Parser::parse()
{

    std::unique_ptr<Expr> expr =  logic_or();
    
    return !panicMode;

}

std::unique_ptr<Expr> Parser::result()
{

    return   logic_or();
       

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
        Error(tokens[current],message);
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
    return logic_or();
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




std::unique_ptr<Expr> Parser::primary()
{
     if (match(TokenType::NOW))
    {
         
          return std::make_unique<NowExpr>();
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
    Error(peek(),"Primary,  Expect expression.");
    return nullptr;
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
        return blockStmt();
    }

    return expressionStatement();
}

std::unique_ptr<Stmt> Parser::printStmt()
{
    consume(TokenType::LEFT_PAREN,"Expect '(' after 'print'.");
    std::unique_ptr<Expr> exp = expression();
    consume(TokenType::RIGHT_PAREN,"Expect ')' after value.");
    consume(TokenType::SEMICOLON,"Expect ';' after value.");
    return std::make_unique<PrintStmt>(std::move(exp));
}

std::unique_ptr<Stmt> Parser::blockStmt()
{
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!check(TokenType::END) && !isAtEnd())
    {
        statements.push_back(declaration());
    }
    consume(TokenType::END,"Expect 'end' after block.");
    return std::make_unique<BlockStmt>(std::move(statements));
}

std::unique_ptr<Stmt> Parser::programStmt()
{
    consume(TokenType::PROGRAM,"Expect 'program' at the beginning.");
    Token name = consume(TokenType::IDENTIFIER,"Expect program name after 'program'.");
    consume(TokenType::SEMICOLON,"Expect ';' after program name.");
    consume(TokenType::BEGIN,"Expect 'begin' after program name.");
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!check(TokenType::END) && !isAtEnd())
    {
        statements.push_back(declaration());
    }
    consume(TokenType::END,"Expect 'end' after block.");
    consume(TokenType::DOT,"Expect '.' after end block.");
    return std::make_unique<Program>(std::move(statements));
   
}

std::unique_ptr<Stmt> Parser::expressionStatement()
{
    std::unique_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON,"Expect ';' after expression.");
    return std::make_unique<ExpressionStmt>(std::move(expr));
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

std::unique_ptr<Stmt> Parser::varDeclaration(LiteralType type)
{
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
    std::unique_ptr<Expr> initializer;
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
        }

    }
    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_unique<VarStmt>(name, std::move(initializer));
}