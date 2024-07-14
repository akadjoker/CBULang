#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Token.hpp"
#include "Literal.hpp"
#include "Exp.hpp"
#include "Stm.hpp"


class Parser
{
public:
    Parser();
    ~Parser();

    void Load(const std::vector<Token> &tokens);

    std::shared_ptr<Expr> result();

    std::shared_ptr<Stmt> parse();

    void clear();

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

     std::shared_ptr<Expr> expression();
     std::shared_ptr<Expr> equality();
     std::shared_ptr<Expr> comparison();
     std::shared_ptr<Expr> assignment();

     std::shared_ptr<Expr> logic_or();
     std::shared_ptr<Expr> logic_and();
     std::shared_ptr<Expr> logic_xor();
     

     std::shared_ptr<Expr> term();  // + -
     std::shared_ptr<Expr> factor(); // * / %
     std::shared_ptr<Expr> power();  // ^ 
     std::shared_ptr<Expr> unary();  // ! -
     std::shared_ptr<Expr> primary(); // number string boolean null variable

     std::shared_ptr<Expr> call( );

     std::shared_ptr<Literal> number();
     std::shared_ptr<Literal> string();

     

     std::shared_ptr<CallerExpr>       functionCall();
     std::shared_ptr<CallerExpr>       nativeFunctionCall();
     std::shared_ptr<CallerExpr>       processCall();
     

     

     // Statements
     std::shared_ptr<ExpressionStmt> expressionStatement();

     std::shared_ptr<EmptyStmt> emptyDeclaration();

     std::shared_ptr<Stmt> declaration();    // declaration of variable in block
     std::shared_ptr<Stmt> defDeclaration(); // declaration of functions/process in program
     std::shared_ptr<Stmt> statement();
     std::shared_ptr<ReturnStmt> returnStmt();

     std::shared_ptr<IfStmt> ifStmt();
     std::shared_ptr<ForStmt> forStmt();
     std::shared_ptr<WhileStmt> whileStmt();
     std::shared_ptr<PrintStmt> printStmt();
     std::shared_ptr<BlockStmt> blockStmt();
     std::shared_ptr<Program> programStmt();
     std::shared_ptr<SwitchStmt> switchStmt();
     std::shared_ptr<RepeatStmt> repeatStmt();
     std::shared_ptr<LoopStmt> loopStmt();
     std::shared_ptr<BreakStmt> breakStmt();
     std::shared_ptr<ContinueStmt> continueStmt();

     std::shared_ptr<ProcedureStmt> procedureStmt();     // declaration of procedure
     std::shared_ptr<ProcedureCallStmt> procedureCall(); // call of procedure

     std::shared_ptr<FunctionStmt> functionStmt();
     
     std::shared_ptr<ProcessStmt> processStmt();
     
     

     std::shared_ptr<VarStmt> varDeclaration(LiteralType type);
   


};