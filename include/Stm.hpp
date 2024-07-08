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
    ELIF,
    PRINT,
    RETURN,
    VAR,
    VARS,
    WHILE,
    FOR,
    REPEAT,
    LOOP,
    SWITCH,
    CONTINUE,
    BREAK,
    PROGRAM,
    STMTCALL,
    PROCEDURECALL,
    COUNT,

};

struct Stmt
{
    unsigned long ID{0};
    


    virtual ~Stmt() {}
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
        case ELIF:
            return "Elif";
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
        case LOOP:
            return "Loop";
        case SWITCH:
            return "Switch";
        case CONTINUE:
            return "Continue";
        case BREAK:
            return "Break";
        case PROGRAM:
            return "Program";
        case STMTCALL:
            return "StmtCall";
        case PROCEDURECALL:
            return "ProcedureCall";

        default:
            return "Unknown";
        }
    }
};

struct EmptyStmt : public Stmt
{
    StmtType getType() const override { return StmtType::EMPTY_STMT; }
    void accept(Visitor *visitor) override;
    EmptyStmt();
};

struct BlockStmt : public Stmt
{
    std::vector<std::shared_ptr<Stmt>> declarations; // variables and call functions/processes

    BlockStmt(std::vector<std::shared_ptr<Stmt>> declarations);

    StmtType getType() const override { return StmtType::BLOCK; }
    void accept(Visitor *visitor) override;
 

};

struct Program : public Stmt
{
    std::string name;
    std::vector<std::shared_ptr<Stmt>> statements; // functions and processes
    std::shared_ptr<Stmt> statement;               // begin end is a block

    Program(const std::string &name, std::vector<std::shared_ptr<Stmt>> statements, std::shared_ptr<Stmt> block);

    StmtType getType() const override { return StmtType::PROGRAM; }
    void accept(Visitor *visitor) override;
    ~Program() 
    {
       // std::cout<<"~Program()"<<std::endl;
    }
   
};

struct ExpressionStmt : public Stmt
{
    std::shared_ptr<Expr> expression;

    ExpressionStmt(std::shared_ptr<Expr> expression);

    StmtType getType() const override { return StmtType::EXPRESSION; }
    void accept(Visitor *visitor) override;
 
};

struct PrintStmt : public Stmt
{
    std::shared_ptr<Expr> expression;

    PrintStmt(std::shared_ptr<Expr> expression);

    StmtType getType() const override { return StmtType::PRINT; }
    void accept(Visitor *visitor) override;
};

struct VarStmt : public Stmt
{
    std::vector<Token> names;
    std::shared_ptr<Expr> initializer;
    LiteralType type;

    VarStmt(std::vector<Token> names, std::shared_ptr<Expr> initializer, LiteralType type);

    StmtType getType() const override { return StmtType::VAR; }
    void accept(Visitor *visitor) override;
};


struct ReturnStmt : public Stmt
{
    std::shared_ptr<Expr> value;

    ReturnStmt(std::shared_ptr<Expr> value);

    StmtType getType() const override { return StmtType::RETURN; }
    void accept(Visitor *visitor) override;
};

struct BreakStmt : public Stmt
{
    BreakStmt();
    StmtType getType() const override { return StmtType::BREAK; }
    void accept(Visitor *visitor) override;
};

struct ContinueStmt : public Stmt
{
    ContinueStmt();
    StmtType getType() const override { return StmtType::CONTINUE; }
    void accept(Visitor *visitor) override;
};

struct Argument
{
    std::string name;
    std::shared_ptr<LiteralExpr> expression;

    Argument(const std::string &name, std::shared_ptr<LiteralExpr> expression) : 
    name(name), expression(std::move(expression)) {}
};

struct ProcedureStmt : public Stmt //declaration
{
    std::string name;
    std::vector<std::shared_ptr<Argument>> parameter;
    std::shared_ptr<Stmt> body;

    ProcedureStmt(const std::string &name, std::vector<std::shared_ptr<Argument>> parameter, std::shared_ptr<Stmt> body);

    StmtType getType() const override { return StmtType::PROCEDURE; }
    void accept(Visitor *visitor) override;

 
};

struct ProcedureCallStmt : public Stmt
{
    Token name;
    std::vector<std::shared_ptr<Expr>> arguments;

    ProcedureCallStmt(const Token &name, std::vector<std::shared_ptr<Expr>> arguments);

    StmtType getType() const override { return StmtType::PROCEDURECALL; }
    void accept(Visitor *visitor) override;
};

struct FunctionStmt : public Stmt
{
    std::string name;
    LiteralType returnType;
    std::vector<std::shared_ptr<Argument>> parameter;
    std::shared_ptr<Stmt> body;

    FunctionStmt(const std::string &name, LiteralType returnType, std::vector<std::shared_ptr<Argument>> parameter, std::shared_ptr<Stmt> body);

    StmtType getType() const override { return StmtType::FUNCTION; }
    void accept(Visitor *visitor) override;
};





struct ElifStmt 
{
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> thenBranch;
    ElifStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch) 
        : condition(std::move(condition)), thenBranch(std::move(thenBranch)) {}
};

struct IfStmt : public Stmt
{
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> thenBranch;
    std::shared_ptr<Stmt> elseBranch;
    std::vector<std::unique_ptr<ElifStmt>> elifBranch;

    IfStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch, std::shared_ptr<Stmt> elseBranch, std::vector<std::unique_ptr<ElifStmt>> elifBranch);

    StmtType getType() const override { return StmtType::IF; }
    void accept(Visitor *visitor) override;
};

struct WhileStmt : public Stmt 
{
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> body;

    WhileStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body);

    StmtType getType() const override { return StmtType::WHILE; }
    void accept(Visitor* visitor) override;
};

struct RepeatStmt : public Stmt
{
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> body;

    RepeatStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body);

    StmtType getType() const override { return StmtType::REPEAT; }
    void accept(Visitor *visitor) override;
};

struct LoopStmt : public Stmt
{
    
    std::shared_ptr<Stmt> body;

    LoopStmt(std::shared_ptr<Stmt> body);

    StmtType getType() const override { return StmtType::LOOP; }
    void accept(Visitor *visitor) override;
};

struct ForStmt : public Stmt
{
    std::shared_ptr<Stmt> initializer;
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Expr> step;
    std::shared_ptr<Stmt> body;

    ForStmt(std::shared_ptr<Stmt> initializer, std::shared_ptr<Expr> condition, std::shared_ptr<Expr> step, std::shared_ptr<Stmt> body);

    StmtType getType() const override { return StmtType::FOR; }
    void accept(Visitor *visitor) override;
};


struct CaseStmt
{
    std::shared_ptr<Expr> value;
    std::shared_ptr<Stmt> body;
    CaseStmt(std::shared_ptr<Expr> value, std::shared_ptr<Stmt> body);
};

struct SwitchStmt : public Stmt
{
    std::shared_ptr<Expr> expression;
    std::shared_ptr<Stmt> default_case;
    std::vector<std::unique_ptr<CaseStmt>> cases;

    SwitchStmt(std::shared_ptr<Expr> expression, std::shared_ptr<Stmt> default_case, std::vector<std::unique_ptr<CaseStmt>> cases);

    StmtType getType() const override { return StmtType::SWITCH; }
    void accept(Visitor *visitor) override;
};

struct ProcessStmt : public Stmt
{
    std::string name;
    std::vector<std::shared_ptr<Argument>> parameter;
    std::shared_ptr<Stmt> body;

    ProcessStmt(std::string name, std::vector<std::shared_ptr<Argument>> parameter, std::shared_ptr<Stmt> body);

    StmtType getType() const override { return StmtType::PROCESS; }
    void accept(Visitor *visitor) override;
};

