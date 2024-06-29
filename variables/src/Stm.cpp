#include "pch.h"
#include "Interpreter.hpp"
#include "Stm.hpp"

std::unique_ptr<Expr> Program::accept(Visitor *visitor)
{
    return visitor->visitProgram(this);
}

std::unique_ptr<Expr> PrintStmt::accept(Visitor *visitor)
{
    return visitor->visitPrintStmt(this);
}

std::unique_ptr<Expr> BlockStmt::accept(Visitor *visitor)
{
    return visitor->visitBlockStmt(this);
}

std::unique_ptr<Expr> ExpressionStmt::accept(Visitor *visitor)
{
    return visitor->visitExpressionStmt(this);
}

std::unique_ptr<Expr> VarStmt::accept(Visitor *visitor)
{
    return visitor->visitVarStmt(this);
}
