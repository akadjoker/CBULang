#include "pch.h"
#include "Interpreter.hpp"
#include "Stm.hpp"

void Program::accept(Visitor *visitor)
{
     visitor->visitProgram(this);
}

void PrintStmt::accept(Visitor *visitor)
{
     visitor->visitPrintStmt(this);
}

void BlockStmt::accept(Visitor *visitor)
{
     visitor->visitBlockStmt(this);
}

void ExpressionStmt::accept(Visitor *visitor)
{
     visitor->visitExpressionStmt(this);
}

void VarStmt::accept(Visitor *visitor)
{
     visitor->visitVarStmt(this);
}

void EmptyStmt::accept(Visitor *visitor)
{
     visitor->visitEmptyStmt(this);
}




void ProcedureStmt::accept(Visitor *visitor)
{
     visitor->visitProcedureStmt(this);
}


void ProcedureCallStmt::accept(Visitor *visitor)
{
     visitor->visitProcedureCallStmt(this);
}

void FunctionStmt::accept(Visitor *visitor)
{
     visitor->visitFunctionStmt(this);
}

void ReturnStmt::accept(Visitor *visitor)
{
     visitor->visitReturnStmt(this);
}


void IfStmt::accept(Visitor *visitor)
{
     visitor->visitIfStmt(this);
}

void WhileStmt::accept(Visitor *visitor)
{
     visitor->visitWhileStmt(this);
}

void BreakStmt::accept(Visitor *visitor)
{
     visitor->visitBreakStmt(this);
}

void ContinueStmt::accept(Visitor *visitor)
{
     visitor->visitContinueStmt(this);
}

void RepeatStmt::accept(Visitor *visitor)
{
     visitor->visitRepeatStmt(this);
}

void LoopStmt::accept(Visitor *visitor)
{
     visitor->visitLoopStmt(this);
}

void SwitchStmt::accept(Visitor *visitor)
{
     visitor->visitSwitchStmt(this);
}

void ForStmt::accept(Visitor *visitor)
{
     visitor->visitForStmt(this);
}
