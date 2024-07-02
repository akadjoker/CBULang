#pragma once
#include "Token.hpp"
#include "Literal.hpp"

class Visitor;

enum StmtType 
{
    EMPTY_STMT = 0,
    BLOCK,
    CLASS,
    EXPRESSION,
    FUNCTION,
    PROCEDURE,
    PROCESS,
    IF,
    PRINT,
    RETURN,
    VAR,
    VARS,
    VARASSIGN,
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
    virtual void accept(Visitor *visitor) = 0;

    std::string toString() const
    {
        switch (getType())
        {
            case EMPTY_STMT:
                return "Empty";
            case BLOCK:
                return "Block";
            case CLASS:
                return "Class";
            case EXPRESSION:
                return "Expression";
            case FUNCTION:
                return "Function";
            case PROCEDURE:
                return "Procedure";
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
            case VARS:
                return "VarAssign";
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

struct EmptyStmt : public Stmt
{
    StmtType getType() const override    {        return StmtType::EMPTY_STMT;    }
    void accept(Visitor *visitor) override;
};


struct BlockStmt : public Stmt
{
    std::vector<std::unique_ptr<Stmt>> declarations;//variables and call functions/processes

    BlockStmt(std::vector<std::unique_ptr<Stmt>> declarations) : declarations(std::move(declarations)) {}
    
    StmtType getType() const override    {        return StmtType::BLOCK;    }
    void accept(Visitor *visitor) override;
};

struct Program : public Stmt
{
    std::string name;
    std::vector<std::unique_ptr<Stmt>> statements;//functions and processes
    std::unique_ptr<Stmt> statement;// begin end is a block

    
    Program(const std::string &name, std::vector<std::unique_ptr<Stmt>> statements, std::unique_ptr<Stmt> block) 
    :name(name), statements(std::move(statements)), statement(std::move(block)) {}
    
    StmtType getType() const override    {        return StmtType::PROGRAM;    }
    void accept(Visitor *visitor) override;
};

struct ExpressionStmt : public Stmt
{
    std::unique_ptr<Expr> expression;
   
    ExpressionStmt(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}
   
    StmtType getType() const override    {        return StmtType::EXPRESSION;    }
    void accept(Visitor *visitor) override;
};

struct PrintStmt : public Stmt
{
    std::unique_ptr<Expr> expression;
    
    PrintStmt(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}
    
    StmtType getType() const override    {        return StmtType::PRINT;    }
    void accept(Visitor *visitor) override;
};

struct VarStmt : public Stmt
{
    Token name;
    std::unique_ptr<Expr> initializer;
    
    VarStmt(Token name, std::unique_ptr<Expr> initializer) : name(name), initializer(std::move(initializer)) {}
    
    StmtType getType() const override    {        return StmtType::VAR;    }
    void accept(Visitor *visitor) override;
};

struct VarsStmt : public Stmt
{
    std::vector<std::string> names;
    std::unique_ptr<Expr> initializer;
    
    VarsStmt(std::unique_ptr<Expr> initializer) : initializer(std::move(initializer)) {}
    
    StmtType getType() const override    {        return StmtType::VARS;    }
   void accept(Visitor *visitor) override;
}; 


struct AssignStmt : public Stmt
{
    std::string name;
    std::unique_ptr<Expr> value;
  

    AssignStmt(const std::string &name, std::unique_ptr<Expr> value) : name(name), value(std::move(value)) {}

    StmtType getType() const override    {        return StmtType::VARASSIGN;    }
     void accept(Visitor *visitor) override;

};