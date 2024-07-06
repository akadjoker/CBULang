#pragma once
#include "Token.hpp"
#include "Literal.hpp"

class Visitor;


enum ExprType 
{
    NONE,
    EMPTY_EXPR,
    BINARY,
    UNARY,
    GROUPING,
    LITERAL,
    VARIABLE,
    ASSIGN,
    LOGICAL,
    NOW,
    CALL,
    NATIVE
};

struct FunctionStmt;

struct Expr 
{
    virtual ~Expr()
    {
       // std::cout<<"Delete Expr"<<std::endl;
    }

    virtual ExprType getType()    const { return ExprType::NONE; }

    virtual std::shared_ptr<Expr> accept(Visitor *visitor) = 0;

    std::string toString() const
    {
        switch (getType())
        {
            case NONE:
                return "None";
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
            case ASSIGN:
                return "Assign";
            case LOGICAL:
                return "Logical";
            case NOW:
                return "Now";
            case CALL:
                return "Call";
            case NATIVE:
                return "Native";   
            default:
                return "Unknow: "+std::to_string((int)getType());
        }
        
    }


};

struct EmptyExpr : public Expr
{
    ExprType getType() const override    {        return ExprType::EMPTY_EXPR;    }

    std::shared_ptr<Expr> accept(Visitor *visitor) override;
};

struct LiteralExpr : public Expr 
{
    std::shared_ptr<Literal> value;

    LiteralExpr(std::shared_ptr<Literal> value) : value(std::move(value)) {}

    ExprType getType() const override     {        return ExprType::LITERAL;    }

    virtual ~LiteralExpr();

    std::shared_ptr<Expr> accept(Visitor *visitor) override;


};

struct BinaryExpr : public Expr
{
    std::shared_ptr<Expr> left;
    std::shared_ptr<Expr> right;
    Token op;

    BinaryExpr(std::shared_ptr<Expr> left, std::shared_ptr<Expr> right, Token op) : left(std::move(left)), right(std::move(right)), op(op) {}

    ExprType getType() const override    {        return ExprType::BINARY;    }
    std::shared_ptr<Expr> accept(Visitor *visitor) override;
};

struct LogicalExpr : public Expr
{
    std::shared_ptr<Expr> left;
    std::shared_ptr<Expr> right;
    Token op;

    LogicalExpr(std::shared_ptr<Expr> left, std::shared_ptr<Expr> right, Token op) : left(std::move(left)), right(std::move(right)), op(op) {}

    ExprType getType() const override    {        return ExprType::LOGICAL;    }
    std::shared_ptr<Expr> accept(Visitor *visitor) override;
};

struct UnaryExpr : public Expr
{
    std::shared_ptr<Expr> right;
    Token op;
    bool isPrefix;

    UnaryExpr(std::shared_ptr<Expr> right, Token op, bool isPrefix=false) : right(std::move(right)), op(op), isPrefix(isPrefix) {}

    ExprType getType() const override    {        return ExprType::UNARY;    }
    std::shared_ptr<Expr> accept(Visitor *visitor) override;
};




struct GroupingExpr : public Expr
{
    std::shared_ptr<Expr> expression;

    GroupingExpr(std::shared_ptr<Expr> expression) : expression(std::move(expression)) {}

    ExprType getType() const override    {        return ExprType::GROUPING;    }
    std::shared_ptr<Expr> accept(Visitor *visitor) override;

};


struct TypeExpr : public Expr
{
    Token name;

    TypeExpr(const Token &name) : name(name) {}

    ExprType getType() const override    {        return ExprType::VARIABLE;    }
    std::shared_ptr<Expr> accept(Visitor *visitor) override;

};



struct VariableExpr : public Expr
{
    Token name;

    VariableExpr(const Token &name) : name(name) {}

    ExprType getType() const override    {        return ExprType::VARIABLE;    }
    std::shared_ptr<Expr> accept(Visitor *visitor) override;

};

struct AssignExpr : public Expr
{
    Token name;
    std::shared_ptr<Expr> value;
    AssignExpr(const Token &name, std::shared_ptr<Expr> value) : name(name), value(std::move(value)) {}

    ExprType getType() const override    {        return ExprType::ASSIGN;    }
    std::shared_ptr<Expr> accept(Visitor *visitor) override;

};



struct NowExpr : public Expr
{
    ExprType getType() const override    {        return ExprType::NOW;    }
    std::shared_ptr<Expr> accept(Visitor *visitor) override;
};


struct FunctionCallExpr : public Expr
{
    Token name;
    std::vector<std::shared_ptr<Expr>> arguments;
    std::shared_ptr<Expr> expression;

   FunctionCallExpr(const Token &name, std::vector<std::shared_ptr<Expr>> arguments, std::shared_ptr<Expr> expr) 
    : name(name), arguments(std::move(arguments)), expression(std::move(expr)) {}


    ExprType getType() const override { return ExprType::CALL; }
    std::shared_ptr<Expr> accept(Visitor *visitor) override;
};



struct NativeFunctionExpr : public Expr
{
    std::string name;
    int line;
    std::vector<std::shared_ptr<Expr>> parameters;
    unsigned int arity;

    NativeFunctionExpr(const std::string &name,int line, std::vector<std::shared_ptr<Expr>> parameters, unsigned int arity)
        : name(name), line(line),parameters(std::move(parameters)), arity(arity) {}

    ExprType getType() const override { return ExprType::NATIVE; }

    std::shared_ptr<Expr> accept(Visitor *visitor) override;
};

