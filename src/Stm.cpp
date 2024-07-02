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

void CallStmt::accept(Visitor *visitor)
{
     std::cout<<"CallStmt"<<std::endl;
     visitor->visitCallStmt(this);
}

void ProcedureCallStmt::accept(Visitor *visitor)
{
     visitor->visitProcedureCallStmt(this);
}
