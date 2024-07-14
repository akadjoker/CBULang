#include "pch.h"
#include "Interpreter.hpp"
#include "Stm.hpp"


static unsigned long StatementID = 0;

Program::Program(const std::string &name, std::vector<std::shared_ptr<Stmt>> statements, std::shared_ptr<Stmt> block):
name(name), statements(std::move(statements)), statement(std::move(block))
{
     StatementID = 0;
     ID = StatementID;
     StatementID++;
}

void Program::accept(Visitor *visitor)
{
     visitor->visitProgram(this);
}

PrintStmt::PrintStmt(std::shared_ptr<Expr> expression)
:expression(std::move(expression))
{
     
     ID = StatementID;
     
}

void PrintStmt::accept(Visitor *visitor)
{
     visitor->visitPrintStmt(this);
}



void BlockStmt::accept(Visitor *visitor)
{
     visitor->visitBlockStmt(this);
}

ExpressionStmt::ExpressionStmt(std::shared_ptr<Expr> expression)
:expression(std::move(expression))
{

     ID = StatementID;
     
}

void ExpressionStmt::accept(Visitor *visitor)
{
     visitor->visitExpressionStmt(this);
}

VarStmt::VarStmt(std::vector<Token> names, std::shared_ptr<Expr> initializer, LiteralType type):    
 names(std::move(names)), initializer(std::move(initializer)), type(type) 
{

     ID = StatementID;
}

void VarStmt::accept(Visitor *visitor)
{
     visitor->visitVarStmt(this);
}

void EmptyStmt::accept(Visitor *visitor)
{
     visitor->visitEmptyStmt(this);
}

ProcedureStmt::ProcedureStmt(const std::string &name, std::vector<std::shared_ptr<Argument>> parameter, std::shared_ptr<Stmt> body):
name(name), parameter(parameter), body(std::move(body)) 
{
    
     ID = StatementID;
     StatementID++;
}

void ProcedureStmt::accept(Visitor *visitor)
{
     visitor->visitProcedureStmt(this);
}

ProcedureCallStmt::ProcedureCallStmt(const Token &name, std::vector<std::shared_ptr<Expr>> arguments):
name(name), arguments(std::move(arguments)) 
{
     ID = StatementID;
}

void ProcedureCallStmt::accept(Visitor *visitor)
{
     visitor->visitProcedureCallStmt(this);
}

FunctionStmt::FunctionStmt(const std::string &name, LiteralType returnType, std::vector<std::shared_ptr<Argument>> parameter, std::shared_ptr<Stmt> body)
: name(name), returnType(returnType), parameter(std::move(parameter)), body(std::move(body)) 
{
     ID = StatementID;
     StatementID++;
}

void FunctionStmt::accept(Visitor *visitor)
{
     visitor->visitFunctionStmt(this);
}

ReturnStmt::ReturnStmt(std::shared_ptr<Expr> value): value(std::move(value)) 
{
     ID = StatementID;
}

void ReturnStmt::accept(Visitor *visitor)
{
     visitor->visitReturnStmt(this);
}

IfStmt::IfStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch, std::shared_ptr<Stmt> elseBranch, std::vector<std::unique_ptr<ElifStmt>> elifBranch)
    : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)), elifBranch(std::move(elifBranch)) 
{
     ID = StatementID;
}

void IfStmt::accept(Visitor *visitor)
{
     visitor->visitIfStmt(this);
}

WhileStmt::WhileStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body)
: condition(std::move(condition)), body(std::move(body)) 
{
     ID = StatementID;
}

void WhileStmt::accept(Visitor *visitor)
{
     visitor->visitWhileStmt(this);
}

BreakStmt::BreakStmt()
{
    ID = StatementID;
}

void BreakStmt::accept(Visitor *visitor)
{
     visitor->visitBreakStmt(this);
}

ContinueStmt::ContinueStmt()
{
    ID = StatementID;
}

void ContinueStmt::accept(Visitor *visitor)
{
     visitor->visitContinueStmt(this);
}

RepeatStmt::RepeatStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body)
: condition(std::move(condition)), body(std::move(body)) 
{
     ID = StatementID;
}

void RepeatStmt::accept(Visitor *visitor)
{
     visitor->visitRepeatStmt(this);
}

LoopStmt::LoopStmt(std::shared_ptr<Stmt> body)
:  body(std::move(body)) 
{

     ID = StatementID;
     StatementID++;
 
     
}

void LoopStmt::accept(Visitor *visitor)
{
     visitor->visitLoopStmt(this);
}

SwitchStmt::SwitchStmt(std::shared_ptr<Expr> expression, std::shared_ptr<Stmt> default_case, std::vector<std::unique_ptr<CaseStmt>> cases)
: expression(std::move(expression)), default_case(std::move(default_case)), cases(std::move(cases)) 
{
     ID = StatementID;
}

void SwitchStmt::accept(Visitor *visitor)
{
     visitor->visitSwitchStmt(this);
}

ForStmt::ForStmt(std::shared_ptr<Stmt> initializer, std::shared_ptr<Expr> condition, std::shared_ptr<Expr> step, std::shared_ptr<Stmt> body)
: initializer(std::move(initializer)), condition(std::move(condition)), step(std::move(step)), body(std::move(body))
{
     ID = StatementID;
}

void ForStmt::accept(Visitor *visitor)
{
     visitor->visitForStmt(this);
}

ProcessStmt::ProcessStmt(std::string name, std::vector<std::shared_ptr<Argument>> parameter, std::shared_ptr<Stmt> body)
: name(std::move(name)), parameter(std::move(parameter)), body(std::move(body)) 
{
     ID = StatementID;
     index = 0;
}

void ProcessStmt::accept(Visitor *visitor)
{
     visitor->visitProcessStmt(this);
}


EmptyStmt::EmptyStmt()
{
    ID = StatementID;
}
BlockStmt::BlockStmt(std::vector<std::shared_ptr<Stmt>> declarations):declarations(std::move(declarations))
{
     
     ID = StatementID;
     StatementID++;
}

CaseStmt::CaseStmt(std::shared_ptr<Expr> value, std::shared_ptr<Stmt> body)
: value(std::move(value)), body(std::move(body)) 
{
     
}
