#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Token.hpp"
#include "Literal.hpp"
#include "Exp.hpp"
#include "Stm.hpp"

// expression     → equality ;
// equality       → comparison ( ( "!=" | "==" ) comparison )* ;
// comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
// term           → factor ( ( "-" | "+" ) factor )* ;
// factor         → unary ( ( "/" | "*" ) unary )* ;
// unary          → ( "!" | "-" ) unary
//                | primary ;
// primary        → NUMBER | STRING | "true" | "false" | "nil"
//                | "(" expression ")" ;

class Parser
{
public:
    Parser(const std::vector<Token> &tokens);
    ~Parser() = default;

    bool parse();
    std::unique_ptr<Expr> result();


private:
    std::vector<Token> tokens;
    int current;
    bool panicMode;


    bool match(std::vector<TokenType> types);
    bool match(TokenType type);
    void consume(TokenType type, const std::string &message);
    bool check(TokenType type);
    bool isAtEnd();
    void synchronize();
    Token advance();
    Token peek();
    Token previous();

    void Error(const Token &token,const std::string &message);
    void Warning(const Token &token,const std::string &message);


    //magic

     std::unique_ptr<Expr> expression();
     std::unique_ptr<Expr> equality();
     std::unique_ptr<Expr> comparison();
   
     std::unique_ptr<Expr> logic_or();
     std::unique_ptr<Expr> logic_and();
     std::unique_ptr<Expr> logic_xor();
     

     std::unique_ptr<Expr> term();  // + -
     std::unique_ptr<Expr> factor(); // * / %
     std::unique_ptr<Expr> power();  // ^ 
     std::unique_ptr<Expr> unary();  // ! -
     std::unique_ptr<Expr> primary(); // number string boolean null variable

     std::unique_ptr<Literal> number();
     std::unique_ptr<Literal> string();
    
};