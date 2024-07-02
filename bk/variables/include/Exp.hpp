#pragma once
#include "Token.hpp"
#include "Literal.hpp"

class Visitor;

enum ExprType 
{
    EMPTY_EXPR,
    BINARY,
    UNARY,
    GROUPING,
    LITERAL,
    VARIABLE,
    ASSIGNMENT,
    LOGICAL,
    NOW
};


struct Expr 
{
    virtual ~Expr()
    {
       // std::cout<<"Delete Expr"<<std::endl;
    }

    virtual ExprType getType() const = 0;

    virtual std::unique_ptr<Expr> accept(Visitor *visitor) = 0;

    std::string toString() const
    {
        switch (getType())
        {
            case EMPTY_EXPR:
                return "Empty";
            case BINARY:
                return "Binary";
            case UNARY:
                return "Unary";
            case GROUPING:
                return "Grouping";
            case LITERAL:
                return "Literal";
            case VARIABLE:
                return "Variable";
            case ASSIGNMENT:
                return "Assignment";
            case LOGICAL:
                return "Logical";
            case NOW:
                return "Now";   
            default:
                return "Unknown";
        }
        
    }


};

struct EmptyExpr : public Expr
{
    ExprType getType() const override    {        return ExprType::EMPTY_EXPR;    }

    std::unique_ptr<Expr> accept(Visitor *visitor) override;
};

struct LiteralExpr : public Expr 
{
    std::unique_ptr<Literal> value;

    LiteralExpr(std::unique_ptr<Literal> value) : value(std::move(value)) {}

    ExprType getType() const override     {        return ExprType::LITERAL;    }

    std::unique_ptr<Expr> accept(Visitor *visitor) override;
};

struct BinaryExpr : public Expr
{
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
    Token op;

    BinaryExpr(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right, Token op) : left(std::move(left)), right(std::move(right)), op(op) {}

    ExprType getType() const override    {        return ExprType::BINARY;    }
    std::unique_ptr<Expr> accept(Visitor *visitor) override;
};

struct LogicalExpr : public Expr
{
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
    Token op;

    LogicalExpr(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right, Token op) : left(std::move(left)), right(std::move(right)), op(op) {}

    ExprType getType() const override    {        return ExprType::LOGICAL;    }
    std::unique_ptr<Expr> accept(Visitor *visitor) override;
};

struct UnaryExpr : public Expr
{
    std::unique_ptr<Expr> right;
    Token op;

    UnaryExpr(std::unique_ptr<Expr> right, Token op) : right(std::move(right)), op(op) {}

    ExprType getType() const override    {        return ExprType::UNARY;    }
    std::unique_ptr<Expr> accept(Visitor *visitor) override;
};

struct GroupingExpr : public Expr
{
    std::unique_ptr<Expr> expression;

    GroupingExpr(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}

    ExprType getType() const override    {        return ExprType::GROUPING;    }
    std::unique_ptr<Expr> accept(Visitor *visitor) override;

};


struct TypeExpr : public Expr
{
    Token name;

    TypeExpr(const Token &name) : name(name) {}

    ExprType getType() const override    {        return ExprType::VARIABLE;    }
    std::unique_ptr<Expr> accept(Visitor *visitor) override;

};



struct VariableExpr : public Expr
{
    Token name;

    VariableExpr(const Token &name) : name(name) {}

    ExprType getType() const override    {        return ExprType::VARIABLE;    }
    std::unique_ptr<Expr> accept(Visitor *visitor) override;

};

struct AssignExpr : public Expr
{
    std::string name;
    std::unique_ptr<LiteralExpr> value;
  

    AssignExpr(const std::string &name, std::unique_ptr<LiteralExpr> value) : name(name), value(std::move(value)) {}

    ExprType getType() const override    {        return ExprType::ASSIGNMENT;    }
    std::unique_ptr<Expr> accept(Visitor *visitor) override;

};

struct NowExpr : public Expr
{
    ExprType getType() const override    {        return ExprType::NOW;    }
    std::unique_ptr<Expr> accept(Visitor *visitor) override;
};