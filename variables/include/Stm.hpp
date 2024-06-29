#pragma once
#include "Token.hpp"
#include "Literal.hpp"

class Visitor;

enum StmtType 
{
    BLOCK,
    CLASS,
    EXPRESSION,
    FUNCTION,
    PROCESS,
    IF,
    PRINT,
    RETURN,
    VAR,
    WHILE,
    FOR,
    REPEAT,
    CONTINUE,
    BREAK,
    PROGRAM,

    
    
};


struct Stmt 
{
    virtual ~Stmt()
    {
       // std::cout<<"Delete Stmt"<<std::endl;
    }

    virtual StmtType getType() const = 0;
    virtual std::unique_ptr<Expr> accept(Visitor *visitor) = 0;

    std::string toString() const
    {
        switch (getType())
        {
            case BLOCK:
                return "Block";
            case CLASS:
                return "Class";
            case EXPRESSION:
                return "Expression";
            case FUNCTION:
                return "Function";
            case PROCESS:
                return "Process";
            case IF:
                return "If";
            case PRINT:
                return "Print";
            case RETURN:
                return "Return";
            case VAR:
                return "Var";
            case WHILE:
                return "While";
            case FOR:
                return "For";
            case REPEAT:
                return "Repeat";
            case CONTINUE:
                return "Continue";
            case BREAK:
                return "Break";
            case PROGRAM:
                return "Program";
            default:
                return "Unknown";
        }
        
    }


};


struct BlockStmt : public Stmt
{
    std::vector<std::unique_ptr<Stmt>> statements;

    BlockStmt(std::vector<std::unique_ptr<Stmt>> statements) : statements(std::move(statements)) {}
    
    StmtType getType() const override    {        return BLOCK;    }
    std::unique_ptr<Expr> accept(Visitor *visitor) override;
};

struct Program : public Stmt
{
    std::vector<std::unique_ptr<Stmt>> statements;
    
    Program(std::vector<std::unique_ptr<Stmt>> statements) : statements(std::move(statements)) {}
    
    StmtType getType() const override    {        return PROGRAM;    }
    std::unique_ptr<Expr> accept(Visitor *visitor) override;
};

struct ExpressionStmt : public Stmt
{
    std::unique_ptr<Expr> expression;
   
    ExpressionStmt(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}
   
    StmtType getType() const override    {        return EXPRESSION;    }
    std::unique_ptr<Expr> accept(Visitor *visitor) override;
};

struct PrintStmt : public Stmt
{
    std::unique_ptr<Expr> expression;
    
    PrintStmt(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}
    
    StmtType getType() const override    {        return PRINT;    }
    std::unique_ptr<Expr> accept(Visitor *visitor) override;
};

struct VarStmt : public Stmt
{
    Token name;
    std::unique_ptr<Expr> initializer;
    
    VarStmt(Token name, std::unique_ptr<Expr> initializer) : name(name), initializer(std::move(initializer)) {}
    
    StmtType getType() const override    {        return VAR;    }
    std::unique_ptr<Expr> accept(Visitor *visitor) override;
};
