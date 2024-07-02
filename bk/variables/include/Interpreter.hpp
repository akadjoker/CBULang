#pragma once

#include "Exp.hpp"
#include "Stm.hpp"


class Visitor
{
public:

    Visitor() = default;
    virtual ~Visitor() = default;



    virtual std::unique_ptr<Expr> visit(std::unique_ptr<Expr>) = 0;    
    
    virtual std::unique_ptr<Expr> visitEmptyExpr(EmptyExpr *expr) = 0;

    virtual std::unique_ptr<Expr> visitBinaryExpr(BinaryExpr *expr) = 0;
    virtual std::unique_ptr<Expr> visitLogicalExpr(LogicalExpr *expr) = 0;
    virtual std::unique_ptr<Expr> visitGroupingExpr(GroupingExpr *expr) = 0;
    virtual std::unique_ptr<Expr> visitLiteralExpr(LiteralExpr *expr) = 0;
    virtual std::unique_ptr<Expr> visitUnaryExpr(UnaryExpr *expr) = 0;
    
    virtual std::unique_ptr<Expr> visitNowExpr(NowExpr *expr) = 0;//current time
    


    virtual std::unique_ptr<Expr> visitVariableExpr(VariableExpr *expr) = 0;
    virtual std::unique_ptr<Expr> visitAssignExpr(AssignExpr *expr) = 0;

    


    virtual void visitPrintStmt(PrintStmt *stmt) = 0;
    virtual void visitVarStmt(VarStmt *stmt) = 0;
    virtual void visitAssignStmt(AssignStmt *stmt) = 0;
    virtual void visitVarsStmt(VarsStmt *stmt) = 0;
    virtual void visitBlockStmt(BlockStmt *stmt) = 0;
    virtual void visitExpressionStmt(ExpressionStmt *stmt) = 0;
    virtual void visitProgram(Program *stmt) = 0;
    virtual void visitEmptyStmt(EmptyStmt *stmt) = 0;//no?? e pk nao 

    
    

};

class Environment
{

private:
    std::unordered_map<std::string, Literal*> m_values;
    unsigned int m_depth ;
    Environment *m_parent ; 

public:
    Environment(int depth, Environment *parent = nullptr);
     
    virtual ~Environment();
    bool define(const std::string &name, Literal *value);
    Literal *get(const std::string &name);
    bool assign(const std::string &name, Literal *value);
    bool contains(const std::string &name);

    unsigned int getDepth() const { return m_depth; }
};

class Interpreter : public Visitor
{
public:

    Interpreter();
    virtual ~Interpreter() ;


    std::unique_ptr<Expr> visit(std::unique_ptr<Expr> expr);
    std::unique_ptr<Expr> visitBinaryExpr(BinaryExpr *expr);
    std::unique_ptr<Expr> visitLogicalExpr(LogicalExpr *expr);
    std::unique_ptr<Expr> visitGroupingExpr(GroupingExpr *expr);
    std::unique_ptr<Expr> visitLiteralExpr(LiteralExpr *expr);
    std::unique_ptr<Expr> visitUnaryExpr(UnaryExpr *expr);
    std::unique_ptr<Expr> visitNowExpr(NowExpr *expr);
    std::unique_ptr<Expr> visitEmptyExpr(EmptyExpr *expr);

    std::unique_ptr<Expr> visitVariableExpr(VariableExpr *expr);
    std::unique_ptr<Expr> visitAssignExpr(AssignExpr *expr);

    std::unique_ptr<Expr> evaluate(std::unique_ptr<Expr> expr);
    std::unique_ptr<Expr> evaluate_ref(std::unique_ptr<Expr> &expr);


    void visitPrintStmt(PrintStmt *stmt);
    void visitVarStmt(VarStmt *stmt);
    void visitVarsStmt(VarsStmt *stmt);
    void visitAssignStmt(AssignStmt *stmt);
    void visitBlockStmt(BlockStmt *stmt);
    void visitExpressionStmt(ExpressionStmt *stmt);
    void visitProgram(Program *stmt);
    void visitEmptyStmt(EmptyStmt *stmt);

    void interpret(std::unique_ptr<Expr> expr);
    
    void interpret(std::vector<std::unique_ptr<Stmt>> &statements);


    void execute(std::unique_ptr<Stmt> &statement);



private:
    bool panicMode;
    unsigned int currentDepth;
    Environment *environment;
    void Error(const Token &token,const std::string &message);
    void Warning(const std::string &message);
    void Info(const std::string &message);
    
    std::unique_ptr<LiteralExpr> createIntLiteral(int value);
    std::unique_ptr<LiteralExpr> createFloatLiteral(float value);
    std::unique_ptr<LiteralExpr> createStringLiteral(const std::string &value);
    std::unique_ptr<LiteralExpr> createBoolLiteral(bool value);
    std::unique_ptr<LiteralExpr> createPointerLiteral(void *value);

    void executeBlock(BlockStmt *stmt, Environment *environment);

    
};