#include "pch.h"
#include "Interpreter.hpp"
#include "Literal.hpp"


//**************************************************************** */

Printer::Printer()
{
    text = "";
}

std::unique_ptr<Expr> Printer::visit(std::unique_ptr<Expr> expr)
{
    return expr->accept(this);
}

void Printer::printOP(TokenType type)
{
    std::string op = "";

    if (type == TokenType::PLUS)
    {
        op = "+";
    }
    else if (type == TokenType::MINUS)
    {
        op = "-";
    }
    else if (type == TokenType::SLASH)
    {
        op = "/";
    }
    else if (type == TokenType::MOD)
    {
        op = "%";
    }
    else if (type == TokenType::STAR)
    {
        op = "*";
    }
    else if (type == TokenType::POWER)
    {
        op = "^";
    }
    else if (type == TokenType::GREATER)
    {
        op = ">";
    }
    else if (type == TokenType::GREATER_EQUAL)
    {
        op = ">=";
    }
    else if (type == TokenType::LESS)
    {
        op = "<";
    }
    else if (type == TokenType::LESS_EQUAL)
    {
        op = "<=";
    }
    else if (type == TokenType::BANG_EQUAL)
    {
        op = "!=";
    }
    else if (type == TokenType::EQUAL_EQUAL)
    {
        op = "==";
    }

    text += op;
}

std::unique_ptr<Expr> Printer::visitBinaryExpr(BinaryExpr *expr)
{
    std::unique_ptr<Expr> left  = std::move(expr->left);
    std::unique_ptr<Expr> right = std::move(expr->right);
    text += "Binary: (";


    if (left->getType() ==ExprType::LITERAL && right->getType() == ExprType::LITERAL)
    {
        LiteralExpr *leftVal = dynamic_cast<LiteralExpr*>(left.get());
        LiteralExpr *rightVal = dynamic_cast<LiteralExpr*>(right.get());

       
        visitLiteralExpr(leftVal);
        text += " ";
        printOP(expr->op.type);
        text += " ";
        visitLiteralExpr(rightVal);
        text += ")";
    } else if (left->getType() == ExprType::BINARY)
    {
        BinaryExpr *leftVal = dynamic_cast<BinaryExpr*>(left.get());

        visitBinaryExpr(leftVal);
        text += " ";
        printOP(expr->op.type);
        text += ")";
    }
    else if (right->getType() == ExprType::GROUPING)
    {

        GroupingExpr *rightVal = dynamic_cast<GroupingExpr*>(right.get());
        text += " ";
        printOP(expr->op.type);
        text += " ";
        visitGroupingExpr(rightVal);
        text += ")";

    } else if (left->getType() == ExprType::GROUPING)
    {
        GroupingExpr *leftVal = dynamic_cast<GroupingExpr*>(left.get());
        text += " ";
        printOP(expr->op.type);
        text += " ";
        visitGroupingExpr(leftVal);
        text += ")";
    } else if (right->getType() == ExprType::BINARY)
    {
        BinaryExpr *rightVal = dynamic_cast<BinaryExpr*>(right.get());
        text += " ";
        printOP(expr->op.type);
        text += " ";
        visitBinaryExpr(rightVal);
        text += ")";
    } else 
    {
        text += "Unknown";
        text += ")";
    }

    return nullptr;
}

std::unique_ptr<Expr> Printer::visitLogicalExpr(LogicalExpr *expr)
{
    text += "Logical: (";
    expr->left->accept(this);
    text += " ";
    printOP(expr->op.type);
    text += " ";
    expr->right->accept(this);
    text += ")";

    return nullptr;
}

std::unique_ptr<Expr> Printer::visitGroupingExpr(GroupingExpr *expr)
{
    text += "Group: (";
    expr->expression->accept(this);
    text += ")";
    return nullptr;
}


std::unique_ptr<Expr> Printer::visitLiteralExpr(LiteralExpr *expr)
{
    text += "Literal: (";
    text += expr->value->toString();
    text += ")";
    return nullptr;
}


std::unique_ptr<Expr> Printer::visitUnaryExpr(UnaryExpr *expr)
{
     text += "Unary: (";
    std::unique_ptr<Expr> right = std::move(expr->right);
    if (right->getType() == ExprType::LITERAL)
    {
        LiteralExpr *rightVal = dynamic_cast<LiteralExpr*>(right.get());
       
        printOP(expr->op.type);
        text += " ";
        visitLiteralExpr(rightVal);
        text += ")\n";
    }
    
    return nullptr;
}

std::unique_ptr<Expr> Printer::visitNowExpr(NowExpr *expr)
{
    return std::unique_ptr<Expr>();
}

std::unique_ptr<Expr> Printer::visitPrintStmt(PrintStmt *stmt)
{
    return std::unique_ptr<Expr>();
}

std::unique_ptr<Expr> Printer::visitVarStmt(VarStmt *stmt)
{
    return std::unique_ptr<Expr>();
}

std::unique_ptr<Expr> Printer::visitBlockStmt(BlockStmt *stmt)
{
    return std::unique_ptr<Expr>();
}

std::unique_ptr<Expr> Printer::visitExpressionStmt(ExpressionStmt *stmt)
{
    return std::unique_ptr<Expr>();
}

std::unique_ptr<Expr> Printer::visitProgram(Program *stmt)
{
    return std::unique_ptr<Expr>();
}

void Printer::print(std::unique_ptr<Expr> expr)
{
    visit(std::move(expr));
    std::cout << "Start(" << std::endl;
    std::cout << text << std::endl;
    std::cout << ")End" << std::endl;
}


/***************************************************************************************** */
std::unique_ptr<Expr> Interpreter::visit(std::unique_ptr<Expr> expr)
{
    return expr->accept(this);
}

std::unique_ptr<Expr> Interpreter::visitBinaryExpr(BinaryExpr *expr)
{

    auto leftVal = evaluate(std::move(expr->left));
    auto rightVal = evaluate(std::move(expr->right));

    if (leftVal->getType() == ExprType::LITERAL && rightVal->getType() == ExprType::LITERAL)
    {
        LiteralExpr *leftLiteral = dynamic_cast<LiteralExpr*>(leftVal.get());
        LiteralExpr *rightLiteral = dynamic_cast<LiteralExpr*>(rightVal.get());

        if (expr->op.type == TokenType::PLUS) // + addition
        {
            if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createIntLiteral(leftLiteral->value->getIntValue() + rightLiteral->value->getIntValue());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createFloatLiteral(leftLiteral->value->getFloatValue() + rightLiteral->value->getFloatValue());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createFloatLiteral(leftLiteral->value->getFloatValue() + rightLiteral->value->getIntValue());
            }
            else if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createFloatLiteral(leftLiteral->value->getIntValue() + rightLiteral->value->getFloatValue());
            }
        }
        else if (expr->op.type == TokenType::MINUS) // - subtraction
        {
            if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createIntLiteral(leftLiteral->value->getIntValue() - rightLiteral->value->getIntValue());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createFloatLiteral(leftLiteral->value->getFloatValue() - rightLiteral->value->getFloatValue());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createFloatLiteral(leftLiteral->value->getFloatValue() - rightLiteral->value->getIntValue());
            }
            else if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createFloatLiteral(leftLiteral->value->getIntValue() - rightLiteral->value->getFloatValue());
            }
        }
        else if (expr->op.type == TokenType::STAR)// * multiplication
        {
            if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createIntLiteral(leftLiteral->value->getIntValue() * rightLiteral->value->getIntValue());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createFloatLiteral(leftLiteral->value->getFloatValue() * rightLiteral->value->getFloatValue());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createFloatLiteral(leftLiteral->value->getFloatValue() * rightLiteral->value->getIntValue());
            }
            else if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createFloatLiteral(leftLiteral->value->getIntValue() * rightLiteral->value->getFloatValue());
            }
        }
        else if (expr->op.type == TokenType::SLASH)// / division
        {
            if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                if (rightLiteral->value->getIntValue() == 0)
                {
                    Error(expr->op, "Division by zero");
                    return createIntLiteral(0);
                }
                return createIntLiteral(leftLiteral->value->getIntValue() / rightLiteral->value->getIntValue());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                if (rightLiteral->value->getFloatValue() == 0)
                {
                    Error(expr->op, "Division by zero");
                    return createFloatLiteral(0);
                }
                return createFloatLiteral(leftLiteral->value->getFloatValue() / rightLiteral->value->getFloatValue());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::INT)
            {
                if (rightLiteral->value->getIntValue() == 0)
                {
                    Error(expr->op, "Division by zero");
                    return createFloatLiteral(0);
                }
                return createFloatLiteral(leftLiteral->value->getFloatValue() / rightLiteral->value->getIntValue());
            }
            else if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                if (rightLiteral->value->getFloatValue() == 0)
                {
                    Error(expr->op, "Division by zero");
                    return createFloatLiteral(0);
                }
                return createFloatLiteral(leftLiteral->value->getIntValue() / rightLiteral->value->getFloatValue());
            }
        }else if (expr->op.type == TokenType::MOD)
        {
            if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createIntLiteral(leftLiteral->value->getIntValue() % rightLiteral->value->getIntValue());
            } else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createFloatLiteral(fmod(leftLiteral->value->getFloatValue(), rightLiteral->value->getFloatValue()));
            }else 
            {
                Error(expr->op, "Unknown binary type (" + leftVal->toString() + " % " + rightVal->toString() + ")");
                return nullptr;
            }
        }else if (expr->op.type == TokenType::POWER)
        {
          if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createIntLiteral(static_cast<int>(std::pow(leftLiteral->value->getIntValue(), rightLiteral->value->getIntValue())));
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createFloatLiteral(std::pow(leftLiteral->value->getFloatValue(), rightLiteral->value->getFloatValue()));
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createFloatLiteral(std::pow(leftLiteral->value->getFloatValue(), rightLiteral->value->getIntValue()));
            }
            else if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createFloatLiteral(std::pow(leftLiteral->value->getIntValue(), rightLiteral->value->getFloatValue()));
            }else
            {
                Error(expr->op, "Unknown binary type (" + leftVal->toString() + " ^ " + rightVal->toString() + ")");
                return nullptr;
            }
        }else if (expr->op.type == TokenType::EQUAL_EQUAL)
        {
            if (leftLiteral->value->getType() != rightLiteral->value->getType() )
            {
                return createBoolLiteral(false);
            }else if (leftLiteral->value->getType() == LiteralType::BOOLEAN && rightLiteral->value->getType() == LiteralType::BOOLEAN)
            {
                return createBoolLiteral(leftLiteral->value->getBoolValue() == rightLiteral->value->getBoolValue());
            } else if (leftLiteral->value->getType() == LiteralType::STRING && rightLiteral->value->getType() == LiteralType::STRING)
            {
                return createBoolLiteral(leftLiteral->value->getStringValue() == rightLiteral->value->getStringValue());
            }
            else if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createBoolLiteral(leftLiteral->value->getIntValue() == rightLiteral->value->getIntValue());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createBoolLiteral(leftLiteral->value->getFloatValue() == rightLiteral->value->getFloatValue());
            }
            else
            {
                Error(expr->op, "Unknown binary type (" + leftVal->toString() + " == " + rightVal->toString() + ")");
                return nullptr;
            }
        }else if (expr->op.type == TokenType::BANG_EQUAL)
        {
            if (leftLiteral->value->getType() != rightLiteral->value->getType() )
            {
                return createBoolLiteral(false);
            }else if (leftLiteral->value->getType() == LiteralType::BOOLEAN && rightLiteral->value->getType() == LiteralType::BOOLEAN)
            {
                return createBoolLiteral(leftLiteral->value->getBoolValue() != rightLiteral->value->getBoolValue());
            } else if (leftLiteral->value->getType() == LiteralType::STRING && rightLiteral->value->getType() == LiteralType::STRING)
            {
                return createBoolLiteral(leftLiteral->value->getStringValue() != rightLiteral->value->getStringValue());
            } else if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createBoolLiteral(leftLiteral->value->getIntValue() != rightLiteral->value->getIntValue());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createBoolLiteral(leftLiteral->value->getFloatValue() != rightLiteral->value->getFloatValue());
            }
            else
            {
                Error(expr->op, "Unknown binary type (" + leftVal->toString() + " != " + rightVal->toString() + ")");
                return nullptr;
            }
        }else if (expr->op.type == TokenType::LESS)// <
        {
            if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createBoolLiteral(leftLiteral->value->getIntValue() < rightLiteral->value->getIntValue());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createBoolLiteral(leftLiteral->value->getFloatValue() < rightLiteral->value->getFloatValue());
            }
            else
            {
                Error(expr->op, "Unknown binary type (" + leftVal->toString() + " < " + rightVal->toString() + ")");
                return nullptr;
            }
         }else if (expr->op.type == TokenType::LESS_EQUAL)// <=
        {
            if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createBoolLiteral(leftLiteral->value->getIntValue() <= rightLiteral->value->getIntValue());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createBoolLiteral(leftLiteral->value->getFloatValue() <= rightLiteral->value->getFloatValue());
            }
            else
            {
                Error(expr->op, "Unknown binary type (" + leftVal->toString() + " <= " + rightVal->toString() + ")");
                return nullptr;
            }
         }else if (expr->op.type == TokenType::GREATER)// >
        {
            if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createBoolLiteral(leftLiteral->value->getIntValue() > rightLiteral->value->getIntValue());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createBoolLiteral(leftLiteral->value->getFloatValue() > rightLiteral->value->getFloatValue());
            }
            else
            {
                Error(expr->op, "Unknown binary type (" + leftVal->toString() + " > " + rightVal->toString() + ")");
                return nullptr;
            }
         }else if (expr->op.type == TokenType::GREATER_EQUAL)// >=
        {
            if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createBoolLiteral(leftLiteral->value->getIntValue() >= rightLiteral->value->getIntValue());
            }else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createBoolLiteral(leftLiteral->value->getFloatValue() >= rightLiteral->value->getFloatValue());
            }
            else
            {
                Error(expr->op, "Unknown binary type (" + leftVal->toString() + " >= " + rightVal->toString() + ")");
                return nullptr;
            }
        } 

    }
    else
    {
        Error(expr->op, "Unknown binary type (" + leftVal->toString() + " - " + rightVal->toString() + ")");
        return nullptr;
    }

    Warning(expr->op, "Unknown binary type " + expr->op.toString());
    return nullptr;
}



std::unique_ptr<Expr> Interpreter::visitUnaryExpr(UnaryExpr *expr)
{
    auto rightVal = evaluate(std::move(expr->right));

    if (rightVal->getType() == ExprType::LITERAL)
    {
        LiteralExpr *literal = dynamic_cast<LiteralExpr*>(rightVal.get());

        if  (expr->op.type==TokenType::MINUS)
        {
            
                if (literal->value->getType() == LiteralType::INT)
                {
                    return createIntLiteral(-literal->value->getIntValue());
                }
                else if (literal->value->getType() == LiteralType::FLOAT)
                {
                    return createFloatLiteral(-literal->value->getFloatValue());
                }
          
            
        } else if (expr->op.type==TokenType::BANG)
        {
            if (literal->value->getType() == LiteralType::BOOLEAN)
            {
                return createBoolLiteral(!literal->value->getBoolValue());
            }
        } else if (expr->op.type==TokenType::NOT)
        {
            if (literal->value->getType() == LiteralType::BOOLEAN)
            {
                return createBoolLiteral(!literal->value->getBoolValue());
            }
        }
        else 
        {
                Error(expr->op, "Unknown unary operator");
                return nullptr;
        }
    }
    else
    {
        Error(expr->op, "Unary operation on non-literal");
        return nullptr;
    }

    Warning(expr->op, "Failed to evaluate unary expression");
    return nullptr;
}


static float native_now()
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return static_cast<float>(milliseconds);
}

std::unique_ptr<Expr> Interpreter::visitNowExpr(NowExpr *expr)
{
    return createFloatLiteral(native_now());
}

static bool literalToBool(const Literal& literal)
{
    switch (literal.getType())
    {
        case LiteralType::BOOLEAN:
            return literal.getBoolValue();
        case LiteralType::INT:
            return literal.getIntValue() != 0;
        case LiteralType::FLOAT:
            return literal.getFloatValue() != 0.0;
        default:
            throw std::runtime_error("Invalid literal type for logical operation");
    }
}

std::unique_ptr<Expr> Interpreter::visitLogicalExpr(LogicalExpr *expr)
{
    auto leftVal = evaluate(std::move(expr->left));
    auto rightVal = evaluate(std::move(expr->right));

    if (leftVal->getType() == ExprType::LITERAL && rightVal->getType() == ExprType::LITERAL)
    {
        LiteralExpr *leftLiteral = dynamic_cast<LiteralExpr*>(leftVal.get());
        LiteralExpr *rightLiteral = dynamic_cast<LiteralExpr*>(rightVal.get());

            bool leftBool = literalToBool(*leftLiteral->value);
            bool rightBool = literalToBool(*rightLiteral->value);
            if (expr->op.type == TokenType::AND)
            {
                return createBoolLiteral(leftBool && rightBool);
            }
            else if (expr->op.type == TokenType::OR)
            {
                return createBoolLiteral(leftBool || rightBool);
            } else if (expr->op.type == TokenType::XOR)
            {
                return createBoolLiteral(leftBool != rightBool);
            }
            
        
    }
    else
    {
        Error(expr->op, "Logical operation on non-literal");
        return nullptr;
    }

    Warning(expr->op, "Failed to evaluate logical expression");
    return nullptr;
}


std::unique_ptr<Expr> Interpreter::visitGroupingExpr(GroupingExpr *expr)
{
    return evaluate(std::move(expr->expression));
}


std::unique_ptr<Expr> Interpreter::visitLiteralExpr(LiteralExpr *expr)
{
    if (expr->value->getType() == LiteralType::INT)
    {
        return createIntLiteral(expr->value->getIntValue());
    } else if (expr->value->getType() == LiteralType::FLOAT)
    {
        return createFloatLiteral(expr->value->getFloatValue());
    } else if (expr->value->getType() == LiteralType::BOOLEAN)
    {
        return createBoolLiteral(expr->value->getBoolValue());
    } else if (expr->value->getType() == LiteralType::STRING)
    {
        return createStringLiteral(expr->value->getStringValue());
    } else if (expr->value->getType() == LiteralType::POINTER)
    {
       
        return createPointerLiteral(expr->value->getPointerValue());
    }
    Token token(TokenType::ERROR,"",0,0);
    Warning(token,"Unknown literal type");
    return nullptr;
   
}




std::unique_ptr<Expr> Interpreter::evaluate(std::unique_ptr<Expr> expr)
{
    return expr->accept(this);
}

void Interpreter::interpret(std::unique_ptr<Expr> expr)
{
  std::unique_ptr<Expr> result =  evaluate(std::move(expr));
  if (result)
  {
    if (result->getType() == ExprType::LITERAL)
    {
        LiteralExpr *literal = dynamic_cast<LiteralExpr*>(result.get());
        std::cout <<"Result: ("<< literal->value->toString()<<")" << std::endl;
    }
     
  } else 
  {
    std::cout <<"Result: (null)" << std::endl;
  }
}


bool Interpreter::test(std::unique_ptr<Expr> expr, std::unique_ptr<Literal> expected)
{
    std::unique_ptr<Expr> result = evaluate(std::move(expr));

    if (!result || result->getType() != ExprType::LITERAL)
    {
        return false;
    }

    LiteralExpr *literal = dynamic_cast<LiteralExpr*>(result.get());
    if (!literal || literal->value->getType() != expected->getType())
    {
        return false;
    }

    switch (literal->value->getType())
    {
        case LiteralType::INT:
            return literal->value->getIntValue() == expected->getIntValue();
        case LiteralType::FLOAT:
            return literal->value->getFloatValue() == expected->getFloatValue();
        case LiteralType::BOOLEAN:
            return literal->value->getBoolValue() == expected->getBoolValue();
        case LiteralType::STRING:
            return literal->value->getStringValue() == expected->getStringValue();
        case LiteralType::POINTER:
            return literal->value->getPointerValue() == expected->getPointerValue();
        default:
            return false;
    }
}

void Interpreter::interpret(std::vector<std::unique_ptr<Stmt>>& statements)
{
    for (auto& stmt : statements)
    {
        execute(stmt);
    }
}

void Interpreter::execute(std::unique_ptr<Stmt>& statement)
{
    statement->accept(this);
}

//**********************************************************************************     */


 std::unique_ptr<Expr> Interpreter::visitPrintStmt(PrintStmt *stmt)
 {
     auto value = evaluate(std::move(stmt->expression));
     std::cout << value->toString() << std::endl;
     return nullptr;
 }

 std::unique_ptr<Expr> Interpreter::visitVarStmt(VarStmt *stmt)
 {
     //TODO
     return nullptr;
 }

 std::unique_ptr<Expr> Interpreter::visitBlockStmt(BlockStmt *stmt)
 {
     //TODO
     return nullptr;
 }

 std::unique_ptr<Expr> Interpreter::visitExpressionStmt(ExpressionStmt *stmt)
 {
     //TODO
     return nullptr;
 }

 std::unique_ptr<Expr> Interpreter::visitProgram(Program *stmt)
 {
     //TODO
     return nullptr;
 }

//********************************************************************************** */


void Interpreter::Error(const Token &token,const std::string &message)
{
    panicMode = true;
    int line = token.line;
    std::string text ="ERROR: " +message+ " at line: " +std::to_string(line);
    throw std::runtime_error(text);
}

void Interpreter::Warning(const Token &token,const std::string &message)
{
    int line = token.line;
    std::string text ="WARNING: " +message+ " at line: " +std::to_string(line);
    throw std::runtime_error(text);
}

std::unique_ptr<LiteralExpr> Interpreter::createIntLiteral(int value)
{
    return std::make_unique<LiteralExpr>(std::make_unique<Literal>(value));
}

std::unique_ptr<LiteralExpr> Interpreter::createFloatLiteral(float value)
{
    return std::make_unique<LiteralExpr>(std::make_unique<Literal>(value));
}

std::unique_ptr<LiteralExpr> Interpreter::createStringLiteral(const std::string &value)
{
    return std::make_unique<LiteralExpr>(std::make_unique<Literal>(value));
}

std::unique_ptr<LiteralExpr> Interpreter::createBoolLiteral(bool value)
{
   
    return std::make_unique<LiteralExpr>(std::make_unique<Literal>(value));
}

std::unique_ptr<LiteralExpr> Interpreter::createPointerLiteral(void *value)
{
    return std::make_unique<LiteralExpr>(std::make_unique<Literal>(value));
}



