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
// primary        → NUMBER | STRING | "true" | "false" | "nil" ! now
//                | "(" expression ")" ;

class Parser
{
public:
    Parser(const std::vector<Token> &tokens);
    ~Parser() = default;

  
    std::unique_ptr<Expr> result();

    std::unique_ptr<Stmt> parse();



private:
    std::vector<Token> tokens;
    int current;
    bool panicMode;
    int countBegins;
    int countEnds ;


    bool match(std::vector<TokenType> types);
    bool match(TokenType type);
    Token consume(TokenType type, const std::string &message);
    bool check(TokenType type);
    bool isAtEnd();
    void synchronize();
    Token advance();
    Token peek();
    Token previous();
    Token lookAhead();

    void Error(const Token &token,const std::string &message);
    void Warning(const Token &token,const std::string &message);


    // expressions

     std::unique_ptr<Expr> expression();
     std::unique_ptr<Expr> equality();
     std::unique_ptr<Expr> comparison();
     std::unique_ptr<Expr> assignment();

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

    std::unique_ptr<Expr> call();
    std::unique_ptr<Stmt> call_stmt(const std::string &name);

    // Statements
    std::unique_ptr<ExpressionStmt> expressionStatement();

    std::unique_ptr<EmptyStmt> emptyDeclaration();

    std::unique_ptr<Stmt> declaration();    // declaration of variable in block
    std::unique_ptr<Stmt> defDeclaration(); // declaration of functions/process in program
    std::unique_ptr<Stmt> statement();

    std::unique_ptr<PrintStmt> printStmt();
    std::unique_ptr<BlockStmt> blockStmt();
    std::unique_ptr<Program> programStmt();


    std::unique_ptr<ProcedureStmt> procedureStmt();// declaration of procedure
    std::unique_ptr<ProcedureCallStmt> procedureCall(); // call of procedure

    std::unique_ptr<VarStmt> varDeclaration(LiteralType type);


    
};