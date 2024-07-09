#include "pch.h"
#include "Interpreter.hpp"
#include "Literal.hpp"
#include "Utils.hpp"

std::shared_ptr<Expr> Interpreter::visitLogicalExpr(LogicalExpr *expr)
{
    auto leftVal = evaluate(expr->left);
    auto rightVal = evaluate(expr->right);

    if (leftVal->getType() == ExprType::LITERAL && rightVal->getType() == ExprType::LITERAL)
    {
        LiteralExpr *leftLiteral = dynamic_cast<LiteralExpr*>(leftVal.get());
        LiteralExpr *rightLiteral = dynamic_cast<LiteralExpr*>(rightVal.get());

        if (leftLiteral && rightLiteral)
        {
            bool leftBool  = leftLiteral->value.asBool();
            bool rightBool = rightLiteral->value.asBool();

            if (expr->op.type == TokenType::AND)
            {
                return Factory::Instance().createBoolLiteral(leftBool && rightBool);
            }
            else if (expr->op.type == TokenType::OR)
            {
                return Factory::Instance().createBoolLiteral(leftBool || rightBool);
            }
            else if (expr->op.type == TokenType::XOR)
            {
                return Factory::Instance().createBoolLiteral(leftBool != rightBool);
            }
        }
    }

    Error("Logical operation on non-literal or invalid literal types");
    return Factory::Instance().createBoolLiteral(false);
}



std::shared_ptr<Expr> Interpreter::visitGroupingExpr(GroupingExpr *expr)
{
    return evaluate(expr->expression);
}

std::shared_ptr<Expr> Interpreter::ReturnByType(LiteralExpr *expr)
{

       //     return expr->self;  


        if (expr->value.getType() == LiteralType::INT)
        {
            return  Factory::Instance().createIntegerLiteral(expr->value.getInt());
        }
        else if (expr->value.getType() == LiteralType::FLOAT)
        {
            return  Factory::Instance().createFloatLiteral(expr->value.getFloat());
        }
        else if (expr->value.getType() == LiteralType::STRING)
        {
            return  Factory::Instance().createStringLiteral(expr->value.getString());
        }
        else if (expr->value.getType() == LiteralType::BOOLEAN)
        {
            return  Factory::Instance().createBoolLiteral(expr->value.getBool());
        } else if (expr->value.getType() == LiteralType::BYTE)
        {
            return  Factory::Instance().createByteLiteral(expr->value.getByte());
        }
      
     return  Factory::Instance().createBoolLiteral(false);    
}

std::shared_ptr<Expr> Interpreter::visitLiteralExpr(LiteralExpr *expr)
{
    return ReturnByType(expr);
}


 std::shared_ptr<Expr> Interpreter::Addition(LiteralExpr *leftLiteral , LiteralExpr *rightLiteral)
{
    auto leftType = leftLiteral->value.getType();
    auto rightType = rightLiteral->value.getType();

    if (leftType == LiteralType::INT)
    {
        if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() + rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() + static_cast<long>(rightLiteral->value.getFloat()));
        }
        else if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() + rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::FLOAT)
    {
        if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() + rightLiteral->value.getFloat());
        }
        else if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() + rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() + rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::BYTE)
    {
        if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() + rightLiteral->value.getByte());
        }
        else if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() + static_cast<unsigned char>(rightLiteral->value.getInt()));
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() + static_cast<unsigned char>(rightLiteral->value.getFloat()));
        }
    }
    else if (leftType == LiteralType::STRING)
    {
        if (rightType == LiteralType::STRING)
        {
            std::string value = leftLiteral->value.getString() + rightLiteral->value.getString();
            return Factory::Instance().createStringLiteral(value);
        }
        else if (rightType == LiteralType::INT)
        {
            std::string value = leftLiteral->value.getString() +  std::to_string(rightLiteral->value.getInt());
            return Factory::Instance().createStringLiteral(value);
        }
        else if (rightType == LiteralType::FLOAT)
        {
            std::string value = leftLiteral->value.getString() +  std::to_string(rightLiteral->value.getFloat());
            return Factory::Instance().createStringLiteral(value);
        }
        else if (rightType == LiteralType::BYTE)
        {
            std::string value = leftLiteral->value.getString() + std::to_string(rightLiteral->value.getByte());
            return Factory::Instance().createStringLiteral(value);
        }
    }

    Warning( "Unsupported operation (" + leftLiteral->toString() + " add " + rightLiteral->toString() + ")");
    return  ReturnByType(leftLiteral);  
}

std::shared_ptr<Expr> Interpreter::Subtraction(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral)
{
    auto leftType = leftLiteral->value.getType();
    auto rightType = rightLiteral->value.getType();

    if (leftType == LiteralType::INT)
    {
        if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() - rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() - static_cast<long>(rightLiteral->value.getFloat()));
        }
        else if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() - rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::FLOAT)
    {
        if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() - rightLiteral->value.getFloat());
        }
        else if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() - rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() - rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::BYTE)
    {
        if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() - rightLiteral->value.getByte());
        }
        else if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() - static_cast<unsigned char>(rightLiteral->value.getInt()));
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() - static_cast<unsigned char>(rightLiteral->value.getFloat()));
        }
    }
    // Subtraction for STRING type is not meaningful and thus not implemented
    Warning( "Unsupported operation (" + leftLiteral->toString() + " sub " + rightLiteral->toString() + ")");
     return  ReturnByType(leftLiteral);    
}


 std::shared_ptr<Expr> Interpreter::Multiplication(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral)
{
    auto leftType = leftLiteral->value.getType();
    auto rightType = rightLiteral->value.getType();

    if (leftType == LiteralType::INT)
    {
        if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() * rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() * static_cast<long>(rightLiteral->value.getFloat()));
        }
        else if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() * rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::FLOAT)
    {
        if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() * rightLiteral->value.getFloat());
        }
        else if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() * rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() * rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::BYTE)
    {
        if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() * rightLiteral->value.getByte());
        }
        else if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() * static_cast<unsigned char>(rightLiteral->value.getInt()));
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() * static_cast<unsigned char>(rightLiteral->value.getFloat()));
        }
    }
    // Multiplication for STRING type is not meaningful and thus not implemented
    Warning( "Unsupported operation (" + leftLiteral->toString() + " mult " + rightLiteral->toString() + ")");
    return  ReturnByType(leftLiteral);     
}

 std::shared_ptr<Expr> Interpreter::Division(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral)
{
    auto leftType = leftLiteral->value.getType();
    auto rightType = rightLiteral->value.getType();

    if ((rightType == LiteralType::INT && rightLiteral->value.getInt() == 0) ||
        (rightType == LiteralType::FLOAT && rightLiteral->value.getFloat() == 0.0) ||
        (rightType == LiteralType::BYTE && rightLiteral->value.getByte() == 0))
    {
        Warning( "Division by zero");
         return  ReturnByType(leftLiteral);  
    }

    if (leftType == LiteralType::INT)
    {
        if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() / rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() / static_cast<long>(rightLiteral->value.getFloat()));
        }
        else if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() / rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::FLOAT)
    {
        if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() / rightLiteral->value.getFloat());
        }
        else if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() / rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() / rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::BYTE)
    {
        if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() / rightLiteral->value.getByte());
        }
        else if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() / static_cast<unsigned char>(rightLiteral->value.getInt()));
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() / static_cast<unsigned char>(rightLiteral->value.getFloat()));
        }
    }
    // Division for STRING type is not meaningful and thus not implemented
    Warning( "Unsupported operation (" + leftLiteral->toString() + " div " + rightLiteral->toString() + ")");
    return  ReturnByType(leftLiteral);  
}



std::shared_ptr<Expr> Interpreter::Modulus(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral)
{
    auto leftType = leftLiteral->value.getType();
    auto rightType = rightLiteral->value.getType();

    if ((rightType == LiteralType::INT && rightLiteral->value.getInt() == 0) ||
        (rightType == LiteralType::FLOAT && rightLiteral->value.getFloat() == 0.0) ||
        (rightType == LiteralType::BYTE && rightLiteral->value.getByte() == 0))
    {
        
         Warning("Division by zero in modulus operation");
         return  ReturnByType(leftLiteral);  
        
    }

    if (leftType == LiteralType::INT)
    {
        if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() % rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() % static_cast<long>(rightLiteral->value.getFloat()));
        }
        else if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() % rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::FLOAT)
    {
        if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createFloatLiteral(std::fmod(leftLiteral->value.getFloat(), rightLiteral->value.getFloat()));
        }
        else if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createFloatLiteral(std::fmod(leftLiteral->value.getFloat(), rightLiteral->value.getInt()));
        }
        else if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createFloatLiteral(std::fmod(leftLiteral->value.getFloat(), rightLiteral->value.getByte()));
        }
    }
    else if (leftType == LiteralType::BYTE)
    {
        if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() % rightLiteral->value.getByte());
        }
        else if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() % static_cast<unsigned char>(rightLiteral->value.getInt()));
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() % static_cast<unsigned char>(rightLiteral->value.getFloat()));
        }
    }
    // Modulus for STRING type is not meaningful and thus not implemented
    Warning( "Unsupported operation (" + leftLiteral->toString() + " mod " + rightLiteral->toString() + ")");
    return  ReturnByType(leftLiteral);  
}



std::shared_ptr<Expr> Interpreter::Power(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral)
{
    auto leftType = leftLiteral->value.getType();
    auto rightType = rightLiteral->value.getType();

    if (leftType == LiteralType::INT)
    {
        if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createIntegerLiteral(static_cast<long>(std::pow(leftLiteral->value.getInt(), rightLiteral->value.getInt())));
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createIntegerLiteral(static_cast<long>(std::pow(leftLiteral->value.getInt(), rightLiteral->value.getFloat())));
        }
        else if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createIntegerLiteral(static_cast<long>(std::pow(leftLiteral->value.getInt(), rightLiteral->value.getByte())));
        }
    }
    else if (leftType == LiteralType::FLOAT)
    {
        if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createFloatLiteral(std::pow(leftLiteral->value.getFloat(), rightLiteral->value.getFloat()));
        }
        else if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createFloatLiteral(std::pow(leftLiteral->value.getFloat(), rightLiteral->value.getInt()));
        }
        else if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createFloatLiteral(std::pow(leftLiteral->value.getFloat(), rightLiteral->value.getByte()));
        }
    }
    else if (leftType == LiteralType::BYTE)
    {
        if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createByteLiteral(static_cast<unsigned char>(std::pow(leftLiteral->value.getByte(), rightLiteral->value.getByte())));
        }
        else if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createByteLiteral(static_cast<unsigned char>(std::pow(leftLiteral->value.getByte(), rightLiteral->value.getInt())));
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createByteLiteral(static_cast<unsigned char>(std::pow(leftLiteral->value.getByte(), rightLiteral->value.getFloat())));
        }
    }

    Warning( "Unsupported operation (" + leftLiteral->toString() + " pow " + rightLiteral->toString() + ")");
    return  ReturnByType(leftLiteral);
}

std::shared_ptr<Expr> Interpreter::EqualEqual(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral)
{
    auto leftType = leftLiteral->value.getType();
    auto rightType = rightLiteral->value.getType();

    if (leftType == LiteralType::BOOLEAN && rightType == LiteralType::BOOLEAN)
    {
        return  Factory::Instance().createBoolLiteral(leftLiteral->value.getBool() == rightLiteral->value.getBool());
    }
    else if (leftType == LiteralType::STRING && rightType == LiteralType::STRING)
    {
        return  Factory::Instance().createBoolLiteral(leftLiteral->value.getString() == rightLiteral->value.getString());
    }
    else if (leftType == LiteralType::INT)
    {
        if (rightType == LiteralType::INT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getInt() == rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getInt() == static_cast<long>(rightLiteral->value.getFloat()));
        }
        else if (rightType == LiteralType::BYTE)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getInt() == rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::FLOAT)
    {
        if (rightType == LiteralType::FLOAT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getFloat() == rightLiteral->value.getFloat());
        }
        else if (rightType == LiteralType::INT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getFloat() == rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::BYTE)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getFloat() == rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::BYTE)
    {
        if (rightType == LiteralType::BYTE)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getByte() == rightLiteral->value.getByte());
        }
        else if (rightType == LiteralType::INT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getByte() == rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getByte() == rightLiteral->value.getFloat());
        }
    }

    Error("Unsupported operation (" + leftLiteral->toString() + " equal " + rightLiteral->toString() + ")");
    return  Factory::Instance().createBoolLiteral(false);
}
std::shared_ptr<Expr> Interpreter::Greater(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral)
{
    auto leftType = leftLiteral->value.getType();
    auto rightType = rightLiteral->value.getType();

    if (leftType == LiteralType::INT)
    {
        if (rightType == LiteralType::INT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getInt() > rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getInt() > static_cast<long>(rightLiteral->value.getFloat()));
        }
        else if (rightType == LiteralType::BYTE)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getInt() > rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::FLOAT)
    {
        if (rightType == LiteralType::FLOAT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getFloat() > rightLiteral->value.getFloat());
        }
        else if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createBoolLiteral(leftLiteral->value.getFloat() > rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::BYTE)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getFloat() > rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::BYTE)
    {
        if (rightType == LiteralType::BYTE)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getByte() > rightLiteral->value.getByte());
        }
        else if (rightType == LiteralType::INT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getByte() > rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getByte() > rightLiteral->value.getFloat());
        }
    }

    Error("Unsupported operation (" + leftLiteral->toString() + " greater than " + rightLiteral->toString() + ")");
    return  Factory::Instance().createBoolLiteral(false);
}

std::shared_ptr<Expr> Interpreter::Less(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral)
{
    auto leftType = leftLiteral->value.getType();
    auto rightType = rightLiteral->value.getType();

    if (leftType == LiteralType::INT)
    {
        if (rightType == LiteralType::INT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getInt() < rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getInt() < static_cast<long>(rightLiteral->value.getFloat()));
        }
        else if (rightType == LiteralType::BYTE)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getInt() < rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::FLOAT)
    {
        if (rightType == LiteralType::FLOAT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getFloat() < rightLiteral->value.getFloat());
        }
        else if (rightType == LiteralType::INT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getFloat() < rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::BYTE)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getFloat() < rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::BYTE)
    {
        if (rightType == LiteralType::BYTE)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getByte() < rightLiteral->value.getByte());
        }
        else if (rightType == LiteralType::INT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getByte() < rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getByte() < rightLiteral->value.getFloat());
        }
    }

    Error("Unsupported operation (" + leftLiteral->toString() + " less than " + rightLiteral->toString() + ")");
    return  Factory::Instance().createBoolLiteral(false);
}

std::shared_ptr<Expr> Interpreter::GreaterEqual(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral)
{
    auto leftType = leftLiteral->value.getType();
    auto rightType = rightLiteral->value.getType();

    if (leftType == LiteralType::INT)
    {
        if (rightType == LiteralType::INT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getInt() >= rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getInt() >= static_cast<long>(rightLiteral->value.getFloat()));
        }
        else if (rightType == LiteralType::BYTE)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getInt() >= rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::FLOAT)
    {
        if (rightType == LiteralType::FLOAT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getFloat() >= rightLiteral->value.getFloat());
        }
        else if (rightType == LiteralType::INT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getFloat() >= rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::BYTE)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getFloat() >= rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::BYTE)
    {
        if (rightType == LiteralType::BYTE)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getByte() >= rightLiteral->value.getByte());
        }
        else if (rightType == LiteralType::INT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getByte() >= rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getByte() >= rightLiteral->value.getFloat());
        }
    }

    Error("Unsupported operation (" + leftLiteral->toString() + " greater than or equal to " + rightLiteral->toString() + ")");
    return  Factory::Instance().createBoolLiteral(false);
}

std::shared_ptr<Expr> Interpreter::LessEqual(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral)
{
    auto leftType = leftLiteral->value.getType();
    auto rightType = rightLiteral->value.getType();

    if (leftType == LiteralType::INT)
    {
        if (rightType == LiteralType::INT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getInt() <= rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getInt() <= static_cast<long>(rightLiteral->value.getFloat()));
        }
        else if (rightType == LiteralType::BYTE)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getInt() <= rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::FLOAT)
    {
        if (rightType == LiteralType::FLOAT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getFloat() <= rightLiteral->value.getFloat());
        }
        else if (rightType == LiteralType::INT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getFloat() <= rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::BYTE)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getFloat() <= rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::BYTE)
    {
        if (rightType == LiteralType::BYTE)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getByte() <= rightLiteral->value.getByte());
        }
        else if (rightType == LiteralType::INT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getByte() <= rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getByte() <= rightLiteral->value.getFloat());
        }
    }

    Error("Unsupported operation (" + leftLiteral->toString() + " less than or equal to " + rightLiteral->toString() + ")");
    return  Factory::Instance().createBoolLiteral(false);
}


std::shared_ptr<Expr> Interpreter::NotEqual(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral)
{
    auto leftType = leftLiteral->value.getType();
    auto rightType = rightLiteral->value.getType();

    if (leftType == LiteralType::BOOLEAN && rightType == LiteralType::BOOLEAN)
    {
        return  Factory::Instance().createBoolLiteral(leftLiteral->value.getBool() != rightLiteral->value.getBool());
    }
    else if (leftType == LiteralType::STRING && rightType == LiteralType::STRING)
    {
        return  Factory::Instance().createBoolLiteral(leftLiteral->value.getString() != rightLiteral->value.getString());
    }
    else if (leftType == LiteralType::INT)
    {
        if (rightType == LiteralType::INT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getInt() != rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getInt() != static_cast<long>(rightLiteral->value.getFloat()));
        }
        else if (rightType == LiteralType::BYTE)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getInt() != rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::FLOAT)
    {
        if (rightType == LiteralType::FLOAT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getFloat() != rightLiteral->value.getFloat());
        }
        else if (rightType == LiteralType::INT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getFloat() != rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::BYTE)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getFloat() != rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::BYTE)
    {
        if (rightType == LiteralType::BYTE)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getByte() != rightLiteral->value.getByte());
        }
        else if (rightType == LiteralType::INT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getByte() != rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return  Factory::Instance().createBoolLiteral(leftLiteral->value.getByte() != rightLiteral->value.getFloat());
        }
    }

    Error( "Unsupported operation (" + leftLiteral->toString() + " not equal " + rightLiteral->toString() + ")");
    return  Factory::Instance().createBoolLiteral(true);
}

std::shared_ptr<Expr> Interpreter::PlusEqual(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral)
{
    auto leftType = leftLiteral->value.getType();
    auto rightType = rightLiteral->value.getType();

    if (leftType == LiteralType::INT)
    {
        if (rightType == LiteralType::INT)
        {
            return  Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() + rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return  Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() + static_cast<long>(rightLiteral->value.getFloat()));
        }
        else if (rightType == LiteralType::BYTE)
        {
            return  Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() + rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::FLOAT)
    {
        if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() + rightLiteral->value.getFloat());
        }
        else if (rightType == LiteralType::INT)
        {
            return  Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() + rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::BYTE)
        {
            return  Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() + rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::BYTE)
    {
        if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() + rightLiteral->value.getByte());
        }
        else if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() + static_cast<unsigned char>(rightLiteral->value.getInt()));
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() + static_cast<unsigned char>(rightLiteral->value.getFloat()));
        }
    }
    else if (leftType == LiteralType::STRING)
    {
        if (rightType == LiteralType::STRING)
        {
            return Factory::Instance().createStringLiteral(leftLiteral->value.getString() + rightLiteral->value.getString());
        }
        else if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createStringLiteral(leftLiteral->value.getString() + std::to_string(rightLiteral->value.getInt()));
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createStringLiteral(leftLiteral->value.getString() + std::to_string(rightLiteral->value.getFloat()));
        }
        else if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createStringLiteral(leftLiteral->value.getString() + std::to_string(rightLiteral->value.getByte()));
        }
    }

    Error( "Unsupported operation (" + leftLiteral->toString() + " plus equal " + rightLiteral->toString() + ")");
    return  ReturnByType(leftLiteral);
}

std::shared_ptr<Expr> Interpreter::MinusEqual(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral)
{
    auto leftType = leftLiteral->value.getType();
    auto rightType = rightLiteral->value.getType();

    if (leftType == LiteralType::INT)
    {
        if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() - rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() - static_cast<long>(rightLiteral->value.getFloat()));
        }
        else if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() - rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::FLOAT)
    {
        if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() - rightLiteral->value.getFloat());
        }
        else if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() - rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() - rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::BYTE)
    {
        if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() - rightLiteral->value.getByte());
        }
        else if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() - static_cast<unsigned char>(rightLiteral->value.getInt()));
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() - static_cast<unsigned char>(rightLiteral->value.getFloat()));
        }
    }

    Error("Unsupported operation (" + leftLiteral->toString() + " minus equal " + rightLiteral->toString() + ")");
    return  ReturnByType(leftLiteral);
}

std::shared_ptr<Expr> Interpreter::StarEqual(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral )
{
    auto leftType = leftLiteral->value.getType();
    auto rightType = rightLiteral->value.getType();

    if (leftType == LiteralType::INT)
    {
        if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() * rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() * static_cast<long>(rightLiteral->value.getFloat()));
        }
        else if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() * rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::FLOAT)
    {
        if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() * rightLiteral->value.getFloat());
        }
        else if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() * rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() * rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::BYTE)
    {
        if (rightType == LiteralType::BYTE)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() * rightLiteral->value.getByte());
        }
        else if (rightType == LiteralType::INT)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() * static_cast<unsigned char>(rightLiteral->value.getInt()));
        }
        else if (rightType == LiteralType::FLOAT)
        {
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() * static_cast<unsigned char>(rightLiteral->value.getFloat()));
        }
    }

    Error( "Unsupported operation (" + leftLiteral->toString() + " star equal " + rightLiteral->toString() + ")");
    return  ReturnByType(leftLiteral);
}

std::shared_ptr<Expr> Interpreter::SlashEqual(LiteralExpr *leftLiteral, LiteralExpr *rightLiteral, const Token &op)
{
    auto leftType = leftLiteral->value.getType();
    auto rightType = rightLiteral->value.getType();

    if (leftType == LiteralType::INT)
    {
        if (rightType == LiteralType::INT)
        {
            if (rightLiteral->value.getInt() == 0)
            {
                Error(op, "Division by zero");
                return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt());
            }
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() / rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::FLOAT)
        {
            if (rightLiteral->value.getFloat() == 0.0)
            {
                Error(op, "Division by zero");
                return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt());
            }
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() / static_cast<long>(rightLiteral->value.getFloat()));
        }
        else if (rightType == LiteralType::BYTE)
        {
            if (rightLiteral->value.getByte() == 0)
            {
                Error(op, "Division by zero");
                return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt());
            }
            return Factory::Instance().createIntegerLiteral(leftLiteral->value.getInt() / rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::FLOAT)
    {
        if (rightType == LiteralType::FLOAT)
        {
            if (rightLiteral->value.getFloat() == 0.0)
            {
                Error(op, "Division by zero");
                return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat());
            }
            return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() / rightLiteral->value.getFloat());
        }
        else if (rightType == LiteralType::INT)
        {
            if (rightLiteral->value.getInt() == 0)
            {
                Error(op, "Division by zero");
                return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat());
            }
            return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() / rightLiteral->value.getInt());
        }
        else if (rightType == LiteralType::BYTE)
        {
            if (rightLiteral->value.getByte() == 0)
            {
                Error(op, "Division by zero");
                return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat());
            }
            return Factory::Instance().createFloatLiteral(leftLiteral->value.getFloat() / rightLiteral->value.getByte());
        }
    }
    else if (leftType == LiteralType::BYTE)
    {
        if (rightType == LiteralType::BYTE)
        {
            if (rightLiteral->value.getByte() == 0)
            {
                Error(op, "Division by zero");
                return Factory::Instance().createByteLiteral(leftLiteral->value.getByte());
            }
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() / rightLiteral->value.getByte());
        }
        else if (rightType == LiteralType::INT)
        {
            if (rightLiteral->value.getInt() == 0)
            {
                Error(op, "Division by zero");
                return Factory::Instance().createByteLiteral(leftLiteral->value.getByte());
            }
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() / static_cast<unsigned char>(rightLiteral->value.getInt()));
        }
        else if (rightType == LiteralType::FLOAT)
        {
            if (rightLiteral->value.getFloat() == 0.0)
            {
                Error(op, "Division by zero");
                return Factory::Instance().createByteLiteral(leftLiteral->value.getByte());
            }
            return Factory::Instance().createByteLiteral(leftLiteral->value.getByte() / static_cast<unsigned char>(rightLiteral->value.getFloat()));
        }
    }

    Error(op, "Unsupported operation (" + leftLiteral->toString() + " slash equal " + rightLiteral->toString() + ")");
   return  ReturnByType(leftLiteral);
}

std::shared_ptr<Expr> Interpreter::visitBinaryExpr(BinaryExpr *expr)
{

    auto leftVal = evaluate(expr->left);
    if (!leftVal)  return std::make_shared<EmptyExpr>();
    auto rightVal = evaluate(expr->right);
    if (!rightVal)  return std::make_shared<EmptyExpr>();

    if (leftVal->getType() == ExprType::LITERAL && rightVal->getType() == ExprType::LITERAL)
    {
        LiteralExpr *leftLiteral = dynamic_cast<LiteralExpr*>(leftVal.get());
        LiteralExpr *rightLiteral = dynamic_cast<LiteralExpr*>(rightVal.get());

       
        if (expr->op.type == TokenType::PLUS) // + addition
        {
           
            return Addition(leftLiteral, rightLiteral);
        }
        else if (expr->op.type == TokenType::MINUS) // - subtraction
        {
            return Subtraction(leftLiteral, rightLiteral);
        }
        else if (expr->op.type == TokenType::STAR)// * multiplication
        {
            return Multiplication(leftLiteral, rightLiteral);
        }
        else if (expr->op.type == TokenType::SLASH)// / division
        {
            return Division(leftLiteral, rightLiteral);

        }else if (expr->op.type == TokenType::MOD)
        {
            return Modulus(leftLiteral, rightLiteral);

        }else if (expr->op.type == TokenType::POWER)
        {
            return Power(leftLiteral, rightLiteral);

        }else if (expr->op.type == TokenType::EQUAL_EQUAL)
        {
            return EqualEqual(leftLiteral, rightLiteral);
             
        }else if (expr->op.type == TokenType::BANG_EQUAL)
        {
            return NotEqual(leftLiteral, rightLiteral);
            
        }else if (expr->op.type == TokenType::LESS)// <
        {
            return Less(leftLiteral, rightLiteral);

         }else if (expr->op.type == TokenType::LESS_EQUAL)// <=
        {
            return LessEqual(leftLiteral, rightLiteral);

         }else if (expr->op.type == TokenType::GREATER)// >
        {
            return Greater(leftLiteral, rightLiteral);

         }else if (expr->op.type == TokenType::GREATER_EQUAL)// >=
        {
            return GreaterEqual(leftLiteral, rightLiteral);

        } else if (expr->op.type == TokenType::PLUS_EQUAL)// +=
        {
            return PlusEqual(leftLiteral, rightLiteral);

        } else if (expr->op.type == TokenType::MINUS_EQUAL)// -=
        {
            return MinusEqual(leftLiteral, rightLiteral);

        } else if (expr->op.type == TokenType::STAR_EQUAL)// *=
        {
            return StarEqual(leftLiteral, rightLiteral);

        } else if (expr->op.type == TokenType::SLASH_EQUAL)// /=
        {
            return SlashEqual(leftLiteral, rightLiteral, expr->op);
        }

    }
   
    
    Error(expr->op, "Unknown binary type (" + leftVal->toString() + " - " + rightVal->toString() + ")");
    return Factory::Instance().createBoolLiteral(false);
}


std::shared_ptr<Expr> Interpreter::handleMinus(LiteralExpr *literal)
{
    auto type = literal->value.getType();
    if (type == LiteralType::INT)
    {
        return Factory::Instance().createIntegerLiteral(-literal->value.getInt());
    }
    else if (type == LiteralType::FLOAT)
    {
        return Factory::Instance().createFloatLiteral(-literal->value.getFloat());
    }
    else if (type == LiteralType::BYTE)
    {
        return Factory::Instance().createByteLiteral(-literal->value.getByte());
    }
   
    Error("Unary minus applied to non-numeric type");
    return Factory::Instance().createBoolLiteral(false);
    
}

std::shared_ptr<Expr> Interpreter::handleBangNot(LiteralExpr *literal)
{
    auto type = literal->value.getType();
    if (type == LiteralType::BOOLEAN)
    {
        return Factory::Instance().createBoolLiteral(!literal->value.getBool());
    }
    else if (type == LiteralType::INT)
    {
        bool value = (literal->value.getInt() == 0) ? true : false;
        return Factory::Instance().createBoolLiteral(value);
    }
    else if (type == LiteralType::FLOAT)
    {
        bool value = (literal->value.getFloat() == 0.0) ? true : false;
        return Factory::Instance().createBoolLiteral(value);
    }
    else if (type == LiteralType::BYTE)
    {
        bool value = (literal->value.getByte() == 0) ? true : false;
        return Factory::Instance().createBoolLiteral(value);
    }

    Error("Unary NOT applied to non-boolean type");
    return Factory::Instance().createBoolLiteral(false);

}


std::shared_ptr<Expr> Interpreter::handleIncrementDecrement(Literal *literal,bool isPrefix, bool isIncrement)
{
  
    auto type = literal->getType();

    if (type == LiteralType::INT)
    {
        long value = literal->getInt();
        long newValue = isIncrement ? value + 1 : value - 1;
        literal->setInt(newValue);
        return Factory::Instance().createIntegerLiteral(isPrefix ? newValue : value);
    }
    else if (type == LiteralType::FLOAT)
    {
        double value = literal->getFloat();
        double newValue = isIncrement ? value + 1.0 : value - 1.0;
        literal->setFloat(newValue);
        return Factory::Instance().createFloatLiteral(isPrefix ? newValue : value);
    }
    else if (type == LiteralType::BYTE)
    {
        unsigned char value = literal->getByte();
        unsigned char newValue = isIncrement ? value + 1 : value - 1;
        literal->setByte(newValue);
        return Factory::Instance().createByteLiteral(isPrefix ? newValue : value);
    }
  
    if (isIncrement)
         Error( "Unsupported type for increment");
    else 
         Error( "Unsupported type for decrement");

    return Factory::Instance().createBoolLiteral(false);
    
}


std::shared_ptr<Expr> Interpreter::visitUnaryExpr(UnaryExpr *expr)
{
    auto rightVal = evaluate(expr->right);
    bool isPrefix = expr->isPrefix;

    if (rightVal->getType() == ExprType::LITERAL)
    {
        LiteralExpr *literal = dynamic_cast<LiteralExpr*>(rightVal.get());

        if  (expr->op.type==TokenType::MINUS)
        {
         
            return handleMinus(literal);
    
        } else if (expr->op.type==TokenType::BANG)
        {
            return handleBangNot(literal);

        } else if (expr->op.type==TokenType::NOT)
        {
            return handleBangNot(literal);
        } else if (expr->op.type==TokenType::INC)
        {
               

                VariableExpr *variable = static_cast<VariableExpr*>(expr->right.get());
                std::string name = variable->name.lexeme;

                Literal* value = this->currentEnvironment()->get(name);
                if (!value)
                {
                    Error(expr->op, "Can Increment, variable  '" + name + "' ");
                    return std::shared_ptr<Expr>(expr, [](Expr*){});  
                }
                

       
                return handleIncrementDecrement(value,isPrefix, true);

               

                          
            
        } else if (expr->op.type==TokenType::DEC)
        {
            VariableExpr *variable = static_cast<VariableExpr*>(expr->right.get());

                std::string name = variable->name.lexeme;

                Literal *value = this->currentEnvironment()->get(name);
                if (!value)
                {
                    Error(expr->op, "Can Increment, variable  '" + name + "' ");
                    return std::shared_ptr<Expr>(expr, [](Expr*){});  
                }
                

                

                return handleIncrementDecrement(value,isPrefix, false);
    
        }
        else 
        {
                Error(expr->op, "Unknown unary operator");
                return std::shared_ptr<Expr>(expr, [](Expr*){});  
        }
    }

    Error(expr->op, "Unary operation on non-literal");
    return std::shared_ptr<Expr>(expr, [](Expr*){});  
   
}
