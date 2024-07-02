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

void VarsStmt::accept(Visitor *visitor)
{
     visitor->visitVarsStmt(this);
}

void AssignStmt::accept(Visitor *visitor)
{
     visitor->visitAssignStmt(this);
}
