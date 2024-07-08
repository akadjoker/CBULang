#pragma once

#include "Exp.hpp"
#include "Stm.hpp"
#include "Process.hpp"


const bool USE_POOL = false;

class Interpreter;
class ExecutionContext;

using LiteralList = std::shared_ptr<Literal>;
typedef LiteralPtr (*NativeFunction)(ExecutionContext* ctx, int argc, LiteralList* argv);

typedef struct 
{
    const char* name;
    NativeFunction func;
} NativeFuncDef;

class ReturnException : public std::runtime_error
{
public:
    std::shared_ptr<Expr> value;
    explicit ReturnException(std::shared_ptr<Expr> value) : std::runtime_error("Return"), value(value) {}
};

class BreakException : public std::runtime_error
{
public:

    explicit BreakException() : std::runtime_error("Break")  {}
};

class ContinueException : public std::runtime_error
{
public:

    explicit ContinueException() : std::runtime_error("Continue") {}
};

class FatalException : public std::runtime_error
{
public:
    explicit FatalException(std::string message) : std::runtime_error(message) {}
};




class ExecutionContext 
{
public:
    ~ExecutionContext()=default;
    ExecutionContext(Interpreter *interpreter);

    bool isTruthy(const LiteralPtr &value);
    bool isEqual(const LiteralPtr &lhs, const LiteralPtr &rhs);

    bool isString(const LiteralList &value);
    bool isInt(const LiteralList &value);
    bool isFloat(const LiteralList &value);
    bool isBool(const LiteralList &value);
    bool isByte(const LiteralList &value);
    
   
    LiteralPtr asFloat(double value);

    LiteralPtr asByte(unsigned char value);
 
    LiteralPtr asInt(long value);
    LiteralPtr asString(const std::string &value);
    LiteralPtr asString(const char *value);
    LiteralPtr asBool(bool value);

    void Error(const std::string &message);
    void Info(const std::string &message);
private:
    friend class Interpreter;
    
    
    Interpreter *interpreter;
};



class Visitor
{
public:
    Visitor() = default;
    virtual ~Visitor() = default;

    virtual std::shared_ptr<Expr> visit(const std::shared_ptr<Expr> &expr) = 0;

    virtual std::shared_ptr<Expr> visitEmptyExpr(EmptyExpr *expr) = 0;

    virtual std::shared_ptr<Expr> visitBinaryExpr(BinaryExpr *expr) = 0;
    virtual std::shared_ptr<Expr> visitLogicalExpr(LogicalExpr *expr) = 0;
    virtual std::shared_ptr<Expr> visitGroupingExpr(GroupingExpr *expr) = 0;
    virtual std::shared_ptr<Expr> visitLiteralExpr(LiteralExpr *expr) = 0;
    virtual std::shared_ptr<Expr> visitUnaryExpr(UnaryExpr *expr) = 0;

    virtual std::shared_ptr<Expr> visitNowExpr(NowExpr *expr) = 0; // current time

    virtual std::shared_ptr<Expr> visitAssignExpr(AssignExpr *expr) = 0;

    virtual std::shared_ptr<Expr> visitVariableExpr(VariableExpr *expr) = 0;

    virtual std::shared_ptr<Expr> visitFunctionCallExpr(FunctionCallExpr *expr) = 0;
    virtual std::shared_ptr<Expr> visitNativeFunctionExpr(NativeFunctionExpr *expr) = 0;
    virtual std::shared_ptr<Expr> visitProcessCallExpr(ProcessCallExpr *expr) = 0;

    virtual void visitProcedureStmt(ProcedureStmt *stmt) = 0;
    virtual void visitFunctionStmt(FunctionStmt *stmt) = 0;

    virtual void visitPrintStmt(PrintStmt *stmt) = 0;

    virtual void visitVarStmt(VarStmt *stmt) = 0;

    virtual void visitIfStmt(IfStmt *stmt) = 0;
    virtual void visitWhileStmt(WhileStmt *stmt) = 0;
    virtual void visitBreakStmt(BreakStmt *stmt) = 0;
    virtual void visitContinueStmt(ContinueStmt *stmt) = 0;
    virtual void visitRepeatStmt(RepeatStmt *stmt) = 0;
    virtual void visitLoopStmt(LoopStmt *stmt) = 0;
    virtual void visitSwitchStmt(SwitchStmt *stmt) = 0;

    virtual void visitReturnStmt(ReturnStmt *stmt) = 0;
    virtual void visitForStmt(ForStmt *stmt) = 0;

    virtual void visitBlockStmt(BlockStmt *stmt) = 0;
    virtual void visitExpressionStmt(ExpressionStmt *stmt) = 0;
    virtual void visitProgram(Program *stmt) = 0;
    virtual void visitEmptyStmt(EmptyStmt *stmt) = 0;
    virtual void visitProcedureCallStmt(ProcedureCallStmt *stmt) = 0;

    virtual void visitProcessStmt(ProcessStmt *stmt) = 0;

    
};

class LiteralPool
{
private:
    LiteralPool();
    ~LiteralPool();

    std::vector<std::unique_ptr<Literal>> pool;

    size_t maxPoolSize = 1000; // Default maximum pool size

    void releaseLiteral(std::unique_ptr<Literal> literal);
    std::shared_ptr<Literal> acquireLiteral();
    void init(size_t size);

public:
    static LiteralPool &Instance()
    {
        static LiteralPool instance;
        return instance;
    }

    
    std::shared_ptr<Literal> acquireInteger(long value);
    std::shared_ptr<Literal> acquireFloat(double value);
    std::shared_ptr<Literal> acquireByte(unsigned char value);
    std::shared_ptr<Literal> acquireBool(bool value);
    std::shared_ptr<Literal> acquireString(const std::string &value);


    void release(std::shared_ptr<Literal> literal);

    void setMaxPoolSize(size_t size) { maxPoolSize = size; }

    void clear();

    static std::shared_ptr<LiteralExpr> createIntegerLiteral(long value);
    static std::shared_ptr<LiteralExpr> createFloatLiteral(double value);
    static std::shared_ptr<LiteralExpr> createByteLiteral(unsigned char value);
    static std::shared_ptr<LiteralExpr> createStringLiteral(const std::string &value);
    static std::shared_ptr<LiteralExpr> createBoolLiteral(bool value);

};

class Environment
{

private:
    std::unordered_map<std::string, std::shared_ptr<Literal>> m_values;

    unsigned int m_depth;
    Environment *m_parent;

public:
    Environment(int depth, Environment *parent = nullptr);

    virtual ~Environment();

    bool define(const std::string &name,const std::shared_ptr<Literal> &value);

    std::shared_ptr<Literal> get(const std::string &name);

    bool assign(const std::string &name, const std::shared_ptr<Literal> &value);

    bool contains(const std::string &name);

    
    bool addInteger(const std::string &name, long value);
    bool addFloat(const std::string &name, double value);
    bool addByte(const std::string &name, unsigned char value);
    bool addBool(const std::string &name, bool value);
    bool addString(const std::string &name, const std::string &value);


    unsigned int count() const { return m_values.size(); }

    unsigned int getDepth() const { return m_depth; }
};


class Interpreter : public Visitor
{
public:
    Interpreter();
    virtual ~Interpreter();

    std::shared_ptr<Expr> visit(const std::shared_ptr<Expr> &expr);
    std::shared_ptr<Expr> visitBinaryExpr(BinaryExpr *expr);
    std::shared_ptr<Expr> visitLogicalExpr(LogicalExpr *expr);
    std::shared_ptr<Expr> visitGroupingExpr(GroupingExpr *expr);
    std::shared_ptr<Expr> visitLiteralExpr(LiteralExpr *expr);
    std::shared_ptr<Expr> visitUnaryExpr(UnaryExpr *expr);
    std::shared_ptr<Expr> visitNowExpr(NowExpr *expr);
    std::shared_ptr<Expr> visitEmptyExpr(EmptyExpr *expr);
    std::shared_ptr<Expr> visitVariableExpr(VariableExpr *expr);
    std::shared_ptr<Expr> visitAssignExpr(AssignExpr *expr);
    std::shared_ptr<Expr> visitFunctionCallExpr(FunctionCallExpr *expr);
    std::shared_ptr<Expr> visitNativeFunctionExpr(NativeFunctionExpr *expr);
    std::shared_ptr<Expr> visitProcessCallExpr(ProcessCallExpr *expr) ;



    void visitPrintStmt(PrintStmt *stmt);
    void visitVarStmt(VarStmt *stmt);
    void visitBlockStmt(BlockStmt *stmt);
    void visitExpressionStmt(ExpressionStmt *stmt);
    void visitProgram(Program *stmt);
    void visitEmptyStmt(EmptyStmt *stmt);
    void visitProcedureCallStmt(ProcedureCallStmt *stmt);
    void visitReturnStmt(ReturnStmt *stmt);
    void visitIfStmt(IfStmt *stmt);
    void visitWhileStmt(WhileStmt *stmt);
    void visitBreakStmt(BreakStmt *stmt) ;
    void visitContinueStmt(ContinueStmt *stmt) ;
    void visitRepeatStmt(RepeatStmt *stmt) ;
    void visitLoopStmt(LoopStmt *stmt) ;
    void visitSwitchStmt(SwitchStmt *stmt) ;
    void visitForStmt(ForStmt *stmt);



    void visitProcedureStmt(ProcedureStmt *stmt);
    void visitFunctionStmt(FunctionStmt *stmt);

    void visitProcessStmt(ProcessStmt *stmt) ;

    
    bool isTruthy(const std::shared_ptr<Expr> &expr);

    void execute(const std::shared_ptr<Stmt> &statement);
 //   void execute(const std::unique_ptr<Stmt> &statement);

    void execute(Stmt *statement);

    std::shared_ptr<Expr> evaluate(const std::shared_ptr<Expr> &expr);
    //std::shared_ptr<Expr> evaluate(const std::unique_ptr<Expr> &expr);




//taks
  
 

    bool run( );
    void build(std::shared_ptr<Stmt> statement);


    void Error(const Token &token, const std::string &message);
    void Warning(const std::string &message);
    void Info(const std::string &message);
    void Error(const std::string &message);

    bool Equal(LiteralExpr *a, LiteralExpr *b);


    void registerFunction(const std::string &name, NativeFunction function);

    std::shared_ptr<Environment> environment;
    
   
  
    void executeBlock(BlockStmt *stmt, const std::shared_ptr<Environment> &env);
     
     size_t Count() const { return processes.size(); }

private:
    friend class Parser;
    bool panicMode;
    unsigned int currentDepth;
    uintptr_t addressLoop;
    unsigned long BlockID;
    

    


    std::shared_ptr<Literal> native_args[25];
    std::shared_ptr<Environment> globalEnvironment;
    std::stack<std::shared_ptr<Environment>> environmentStack;
    std::shared_ptr<ExecutionContext> context;
    std::chrono::high_resolution_clock::time_point start_time;
    std::vector<std::unique_ptr<Process>> processes;
    std::vector<std::unique_ptr<Process>> remove_processes;


    double time_elapsed();



    NativeFunction getNativeFunction(const std::string &name) const;
    bool isNativeFunctionDefined(const std::string &name) const;

    std::shared_ptr<Expr> Addition(LiteralExpr *leftLiteral , LiteralExpr *rightLiteral);

    std::shared_ptr<Expr> Subtraction(LiteralExpr *leftLiteral , LiteralExpr *rightLiteral);

    std::shared_ptr<Expr> Multiplication(LiteralExpr *leftLiteral , LiteralExpr *rightLiteral);

    std::shared_ptr<Expr> Division(LiteralExpr *leftLiteral , LiteralExpr *rightLiteral);

    std::shared_ptr<Expr> Modulus(LiteralExpr *leftLiteral , LiteralExpr *rightLiteral);

    std::shared_ptr<Expr> Power(LiteralExpr *leftLiteral , LiteralExpr *rightLiteral);
    
    std::shared_ptr<Expr> EqualEqual(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral);

    std::shared_ptr<Expr> NotEqual(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral);

    std::shared_ptr<Expr> Greater(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral);

    std::shared_ptr<Expr> Less(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral);

    std::shared_ptr<Expr> GreaterEqual(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral);

    std::shared_ptr<Expr> LessEqual(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral);


    std::shared_ptr<Expr> PlusEqual(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral);
    std::shared_ptr<Expr> MinusEqual(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral);
    std::shared_ptr<Expr> StarEqual(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral);
    std::shared_ptr<Expr> SlashEqual(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral, const Token &op);

    std::shared_ptr<Expr> handleIncrementDecrement(Literal *literal,bool isPrefix, bool isIncrement);

    std::shared_ptr<Expr> ReturnByType(LiteralExpr *value);

    std::shared_ptr<Expr> handleMinus(LiteralExpr *literal);
    std::shared_ptr<Expr> handleBangNot(LiteralExpr *literal);

    std::unordered_map<std::string, ProcedureStmt *> procedureList;
    std::unordered_map<std::string, FunctionStmt *> functionList;
    std::unordered_map<std::string, ProcessStmt *> processList;

    std::unordered_map<std::string, NativeFunction> nativeFunctions;
    };