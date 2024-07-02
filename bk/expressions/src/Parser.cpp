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

void Parser::consume(TokenType type,const std::string &message)
{
    if (check(type))
    {
        advance();
    }
    else
    {
        Error(tokens[current],message);
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
