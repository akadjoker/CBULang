#pragma once

#include "Exp.hpp"
#include "Stm.hpp"


class Visitor
{
public:

    Visitor() = default;
    virtual ~Visitor() = default;



    virtual std::unique_ptr<Expr> visit(std::unique_ptr<Expr>) = 0;    
    
    virtual std::unique_ptr<Expr> visitBinaryExpr(BinaryExpr *expr) = 0;
    virtual std::unique_ptr<Expr> visitLogicalExpr(LogicalExpr *expr) = 0;
    virtual std::unique_ptr<Expr> visitGroupingExpr(GroupingExpr *expr) = 0;
    virtual std::unique_ptr<Expr> visitLiteralExpr(LiteralExpr *expr) = 0;
    virtual std::unique_ptr<Expr> visitUnaryExpr(UnaryExpr *expr) = 0;
    virtual std::unique_ptr<Expr> visitNowExpr(NowExpr *expr) = 0;


    virtual std::unique_ptr<Expr> visitPrintStmt(PrintStmt *stmt) = 0;
    virtual std::unique_ptr<Expr> visitVarStmt(VarStmt *stmt) = 0;
    virtual std::unique_ptr<Expr> visitBlockStmt(BlockStmt *stmt) = 0;
    virtual std::unique_ptr<Expr> visitExpressionStmt(ExpressionStmt *stmt) = 0;
    virtual std::unique_ptr<Expr> visitProgram(Program *stmt) = 0;

    
    

};

class Printer : public Visitor
{
private:
    std::string text;


 
     void printOP(TokenType op);


public:
    Printer();
    ~Printer() = default;

    //expressions
    std::unique_ptr<Expr> visit(std::unique_ptr<Expr> expr);
    std::unique_ptr<Expr> visitBinaryExpr(BinaryExpr *expr);
    std::unique_ptr<Expr> visitLogicalExpr(LogicalExpr *expr);
    std::unique_ptr<Expr> visitGroupingExpr(GroupingExpr *expr);
    std::unique_ptr<Expr> visitLiteralExpr(LiteralExpr *expr);
    std::unique_ptr<Expr> visitUnaryExpr(UnaryExpr *expr);
    std::unique_ptr<Expr> visitNowExpr(NowExpr *expr);

    //statement

    std::unique_ptr<Expr> visitPrintStmt(PrintStmt *stmt);
    std::unique_ptr<Expr> visitVarStmt(VarStmt *stmt);
    std::unique_ptr<Expr> visitBlockStmt(BlockStmt *stmt);
    std::unique_ptr<Expr> visitExpressionStmt(ExpressionStmt *stmt);
    std::unique_ptr<Expr> visitProgram(Program *stmt);




    void print(std::unique_ptr<Expr> expr);
};



class Interpreter : public Visitor
{
public:
    std::unique_ptr<Expr> visit(std::unique_ptr<Expr> expr);
    std::unique_ptr<Expr> visitBinaryExpr(BinaryExpr *expr);
    std::unique_ptr<Expr> visitLogicalExpr(LogicalExpr *expr);
    std::unique_ptr<Expr> visitGroupingExpr(GroupingExpr *expr);
    std::unique_ptr<Expr> visitLiteralExpr(LiteralExpr *expr);
    std::unique_ptr<Expr> visitUnaryExpr(UnaryExpr *expr);
    std::unique_ptr<Expr> visitNowExpr(NowExpr *expr);

    std::unique_ptr<Expr> evaluate(std::unique_ptr<Expr> expr);


    std::unique_ptr<Expr> visitPrintStmt(PrintStmt *stmt);
    std::unique_ptr<Expr> visitVarStmt(VarStmt *stmt);
    std::unique_ptr<Expr> visitBlockStmt(BlockStmt *stmt);
    std::unique_ptr<Expr> visitExpressionStmt(ExpressionStmt *stmt);
    std::unique_ptr<Expr> visitProgram(Program *stmt);

    void interpret(std::unique_ptr<Expr> expr);
    bool test(std::unique_ptr<Expr> expr, std::unique_ptr<Literal> expected);
    void interpret(std::vector<std::unique_ptr<Stmt>> &statements);
    void execute(std::unique_ptr<Stmt> &statement);

private:
    bool panicMode = false;
    void Error(const Token &token,const std::string &message);
    void Warning(const Token &token,const std::string &message);
    
    std::unique_ptr<LiteralExpr> createIntLiteral(int value);
    std::unique_ptr<LiteralExpr> createFloatLiteral(float value);
    std::unique_ptr<LiteralExpr> createStringLiteral(const std::string &value);
    std::unique_ptr<LiteralExpr> createBoolLiteral(bool value);
    std::unique_ptr<LiteralExpr> createPointerLiteral(void *value);

    
};