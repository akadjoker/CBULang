#include "pch.h"
#include "Interpreter.hpp"
#include "Exp.hpp"
#include "Stm.hpp"
#include "Utils.hpp"

LiteralExpr::~LiteralExpr()
{
   // Log(0, "Delete LiteralExpr");
    if (USE_POOL)
    {
      //  LiteralPool::Instance().release(value);
    }
}

std::shared_ptr<Expr> LiteralExpr::accept(Visitor *visitor)
{
    return visitor->visitLiteralExpr(this);
}

std::shared_ptr<Expr> BinaryExpr::accept(Visitor *visitor)
{
    return visitor->visitBinaryExpr(this);
}

std::shared_ptr<Expr> UnaryExpr::accept(Visitor *visitor)
{
    return visitor->visitUnaryExpr(this);
}

std::shared_ptr<Expr> GroupingExpr::accept(Visitor *visitor)
{
    return visitor->visitGroupingExpr(this);
}
std::shared_ptr<Expr> LogicalExpr::accept(Visitor *visitor) 
{
    return visitor->visitLogicalExpr(this);
}


std::shared_ptr<Expr> VariableExpr::accept(Visitor *visitor)
{
    return visitor->visitVariableExpr(this);
}



std::shared_ptr<Expr> NowExpr::accept(Visitor *visitor)
{
    return visitor->visitNowExpr(this);
}

std::shared_ptr<Expr> EmptyExpr::accept(Visitor *visitor)
{
    return visitor->visitEmptyExpr(this);
}

std::shared_ptr<Expr> AssignExpr::accept(Visitor *visitor)
{
    return visitor->visitAssignExpr(this);
}

std::shared_ptr<Expr> FunctionCallExpr::accept(Visitor *visitor)
{
    return visitor->visitFunctionCallExpr(this);
}

std::shared_ptr<Expr> NativeFunctionExpr::accept(Visitor *visitor)
{
    return visitor->visitNativeFunctionExpr(this);
}

std::shared_ptr<Expr> ProcessCallExpr::accept(Visitor *visitor)
{
    return visitor->visitProcessCallExpr(this);
}
