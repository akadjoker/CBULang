#include "pch.h"
#include "Interpreter.hpp"
#include "Exp.hpp"


std::unique_ptr<Expr> LiteralExpr::accept(Visitor *visitor)
{
    return visitor->visitLiteralExpr(this);
}

std::unique_ptr<Expr> BinaryExpr::accept(Visitor *visitor)
{
    return visitor->visitBinaryExpr(this);
}

std::unique_ptr<Expr> UnaryExpr::accept(Visitor *visitor)
{
    return visitor->visitUnaryExpr(this);
}

std::unique_ptr<Expr> GroupingExpr::accept(Visitor *visitor)
{
    return visitor->visitGroupingExpr(this);
}
std::unique_ptr<Expr> LogicalExpr::accept(Visitor *visitor) 
{
    return visitor->visitLogicalExpr(this);
}


std::unique_ptr<Expr> VariableExpr::accept(Visitor *visitor)
{
    return visitor->visitVariableExpr(this);
}



std::unique_ptr<Expr> NowExpr::accept(Visitor *visitor)
{
    return visitor->visitNowExpr(this);
}

std::unique_ptr<Expr> EmptyExpr::accept(Visitor *visitor)
{
    return visitor->visitEmptyExpr(this);
}

std::unique_ptr<Expr> AssignExpr::accept(Visitor *visitor)
{
    return visitor->visitAssignExpr(this);
}
