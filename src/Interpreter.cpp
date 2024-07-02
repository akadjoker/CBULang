#include "pch.h"
#include "Interpreter.hpp"
#include "Literal.hpp"

Interpreter::Interpreter()
{
    currentDepth = 0;
    environment = new Environment(currentDepth);
    panicMode = false;
}

Interpreter::~Interpreter()
{
    if (environment != nullptr)
    {
      delete environment;
     environment = nullptr;
    }
    
    procedureList.clear();
}


std::unique_ptr<Expr> Interpreter::visit(std::unique_ptr<Expr> expr)
{
    return expr->accept(this);
}

std::unique_ptr<Expr> Interpreter::visitBinaryExpr(BinaryExpr *expr)
{

    auto leftVal = evaluate(std::move(expr->left));
    if (!leftVal) return std::unique_ptr<EmptyExpr>();
    auto rightVal = evaluate(std::move(expr->right));
    if (!rightVal) return std::unique_ptr<EmptyExpr>();

    if (leftVal->getType() == ExprType::LITERAL && rightVal->getType() == ExprType::LITERAL)
    {
        LiteralExpr *leftLiteral = dynamic_cast<LiteralExpr*>(leftVal.get());
        LiteralExpr *rightLiteral = dynamic_cast<LiteralExpr*>(rightVal.get());

        if (expr->op.type == TokenType::PLUS) // + addition
        {
            if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createIntLiteral(leftLiteral->value->getInt() + rightLiteral->value->getInt());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createFloatLiteral(leftLiteral->value->getFloat() + rightLiteral->value->getFloat());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createFloatLiteral(leftLiteral->value->getFloat() + rightLiteral->value->getInt());
            }
            else if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createFloatLiteral(leftLiteral->value->getInt() + rightLiteral->value->getFloat());
            } else if (leftLiteral->value->getType() == LiteralType::STRING && rightLiteral->value->getType() == LiteralType::STRING)
            {
                std::string value = std::string(leftLiteral->value->getString());
                value += std::string(rightLiteral->value->getString());
                return createStringLiteral(value);
            } else if (leftLiteral->value->getType() == LiteralType::STRING && rightLiteral->value->getType() == LiteralType::INT)
            {
                std::string value = leftLiteral->value->getString() + std::to_string(rightLiteral->value->getInt());
                return createStringLiteral(value);
            } else if (leftLiteral->value->getType() == LiteralType::STRING && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                std::string value = leftLiteral->value->getString() + std::to_string(rightLiteral->value->getFloat());
                return createStringLiteral(value);
            } else if (leftLiteral->value->getType() == LiteralType::STRING && rightLiteral->value->getType() == LiteralType::BOOLEAN)
            {
                std::string value = leftLiteral->value->getString() + std::to_string(rightLiteral->value->getBool());
                return createStringLiteral(value);
            } else 
            {
                                
                 Error(expr->op, "Unsupported operation (" + leftVal->toString() + " + " + rightVal->toString() + ")");
                  return createBoolLiteral(false);
                
            }
        }
        else if (expr->op.type == TokenType::MINUS) // - subtraction
        {
            if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createIntLiteral(leftLiteral->value->getInt() - rightLiteral->value->getInt());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createFloatLiteral(leftLiteral->value->getFloat() - rightLiteral->value->getFloat());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createFloatLiteral(leftLiteral->value->getFloat() - rightLiteral->value->getInt());
            }
            else if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createFloatLiteral(leftLiteral->value->getInt() - rightLiteral->value->getFloat());
            } else 
            {
                                
                  Error(expr->op, "Unsupported operation (" + leftVal->toString() + " - " + rightVal->toString() + ")");
                  return createBoolLiteral(false);
                
            }
        }
        else if (expr->op.type == TokenType::STAR)// * multiplication
        {
            if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createIntLiteral(leftLiteral->value->getInt() * rightLiteral->value->getInt());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createFloatLiteral(leftLiteral->value->getFloat() * rightLiteral->value->getFloat());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createFloatLiteral(leftLiteral->value->getFloat() * rightLiteral->value->getInt());
            }
            else if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createFloatLiteral(leftLiteral->value->getInt() * rightLiteral->value->getFloat());
            } else 
            {
                                
                  Error(expr->op, "Unsupported operation (" + leftVal->toString() + " * " + rightVal->toString() + ")");
                  return createBoolLiteral(false);
                
            }
        }
        else if (expr->op.type == TokenType::SLASH)// / division
        {
            if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                if (rightLiteral->value->getInt() == 0)
                {
                    Error(expr->op, "Division by zero");
                    return createIntLiteral(0);
                }
                return createIntLiteral(leftLiteral->value->getInt() / rightLiteral->value->getInt());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                if (rightLiteral->value->getFloat() == 0)
                {
                    Error(expr->op, "Division by zero");
                    return createFloatLiteral(0);
                }
                return createFloatLiteral(leftLiteral->value->getFloat() / rightLiteral->value->getFloat());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::INT)
            {
                if (rightLiteral->value->getInt() == 0)
                {
                    Error(expr->op, "Division by zero");
                    return createFloatLiteral(0);
                }
                return createFloatLiteral(leftLiteral->value->getFloat() / rightLiteral->value->getInt());
            }
            else if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                if (rightLiteral->value->getFloat() == 0)
                {
                    Error(expr->op, "Division by zero");
                    return createFloatLiteral(0);
                }
                return createFloatLiteral(leftLiteral->value->getInt() / rightLiteral->value->getFloat());
            }
        }else if (expr->op.type == TokenType::MOD)
        {
            if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createIntLiteral(leftLiteral->value->getInt() % rightLiteral->value->getInt());
            } else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createFloatLiteral(fmod(leftLiteral->value->getFloat(), rightLiteral->value->getFloat()));
            }else 
            {
                Error(expr->op, "Unsupported operation (" + leftVal->toString() + " % " + rightVal->toString() + ")");
                return createBoolLiteral(false);
            }
        }else if (expr->op.type == TokenType::POWER)
        {
          if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createIntLiteral(static_cast<int>(std::pow(leftLiteral->value->getInt(), rightLiteral->value->getInt())));
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createFloatLiteral(std::pow(leftLiteral->value->getFloat(), rightLiteral->value->getFloat()));
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createFloatLiteral(std::pow(leftLiteral->value->getFloat(), rightLiteral->value->getInt()));
            }
            else if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createFloatLiteral(std::pow(leftLiteral->value->getInt(), rightLiteral->value->getFloat()));
            }else
            {
                Error(expr->op, "Unsupported operation (" + leftVal->toString() + " ^ " + rightVal->toString() + ")");
                return createBoolLiteral(false);
            }
        }else if (expr->op.type == TokenType::EQUAL_EQUAL)
        {
            if (leftLiteral->value->getType() != rightLiteral->value->getType() )
            {
                return createBoolLiteral(false);
            }else if (leftLiteral->value->getType() == LiteralType::BOOLEAN && rightLiteral->value->getType() == LiteralType::BOOLEAN)
            {
                return createBoolLiteral(leftLiteral->value->getBool() == rightLiteral->value->getBool());
            } else if (leftLiteral->value->getType() == LiteralType::STRING && rightLiteral->value->getType() == LiteralType::STRING)
            {
                return createBoolLiteral(leftLiteral->value->getString() == rightLiteral->value->getString());
            }
            else if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createBoolLiteral(leftLiteral->value->getInt() == rightLiteral->value->getInt());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createBoolLiteral(leftLiteral->value->getFloat() == rightLiteral->value->getFloat());
            }
            else
            {
                                
                 Error(expr->op, "Unsupported operation (" + leftVal->toString() + " == " + rightVal->toString() + ")");
                  return createBoolLiteral(false);
                
            }
             
        }else if (expr->op.type == TokenType::BANG_EQUAL)
        {
            if (leftLiteral->value->getType() != rightLiteral->value->getType() )
            {
                return createBoolLiteral(false);
            }else if (leftLiteral->value->getType() == LiteralType::BOOLEAN && rightLiteral->value->getType() == LiteralType::BOOLEAN)
            {
                return createBoolLiteral(leftLiteral->value->getBool() != rightLiteral->value->getBool());
            } else if (leftLiteral->value->getType() == LiteralType::STRING && rightLiteral->value->getType() == LiteralType::STRING)
            {
                return createBoolLiteral(leftLiteral->value->getString() != rightLiteral->value->getString());
            } else if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createBoolLiteral(leftLiteral->value->getInt() != rightLiteral->value->getInt());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createBoolLiteral(leftLiteral->value->getFloat() != rightLiteral->value->getFloat());
            }
            else
            {
                Error(expr->op, "Unsupported operation (" + leftVal->toString() + " != " + rightVal->toString() + ")");
                return createBoolLiteral(false);
            }
        }else if (expr->op.type == TokenType::LESS)// <
        {
            if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createBoolLiteral(leftLiteral->value->getInt() < rightLiteral->value->getInt());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createBoolLiteral(leftLiteral->value->getFloat() < rightLiteral->value->getFloat());
            }
            else
            {
               Error(expr->op, "Unsupported operation (" + leftVal->toString() + " < " + rightVal->toString() + ")");
               return createBoolLiteral(false);
            }
         }else if (expr->op.type == TokenType::LESS_EQUAL)// <=
        {
            if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createBoolLiteral(leftLiteral->value->getInt() <= rightLiteral->value->getInt());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createBoolLiteral(leftLiteral->value->getFloat() <= rightLiteral->value->getFloat());
            }
            else
            {
               Error(expr->op, "Unsupported operation (" + leftVal->toString() + " <= " + rightVal->toString() + ")");
                return createBoolLiteral(false);
            }
         }else if (expr->op.type == TokenType::GREATER)// >
        {
            if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createBoolLiteral(leftLiteral->value->getInt() > rightLiteral->value->getInt());
            }
            else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createBoolLiteral(leftLiteral->value->getFloat() > rightLiteral->value->getFloat());
            }
            else
            {
                Error(expr->op, "Unsupported operation (" + leftVal->toString() + " > " + rightVal->toString() + ")");
                return createBoolLiteral(false);
            }
         }else if (expr->op.type == TokenType::GREATER_EQUAL)// >=
        {
            if (leftLiteral->value->getType() == LiteralType::INT && rightLiteral->value->getType() == LiteralType::INT)
            {
                return createBoolLiteral(leftLiteral->value->getInt() >= rightLiteral->value->getInt());
            }else if (leftLiteral->value->getType() == LiteralType::FLOAT && rightLiteral->value->getType() == LiteralType::FLOAT)
            {
                return createBoolLiteral(leftLiteral->value->getFloat() >= rightLiteral->value->getFloat());
            }
            else
            {
               Error(expr->op, "Unsupported operation (" + leftVal->toString() + " >= " + rightVal->toString() + ")");
                return createBoolLiteral(false);
            }
        }

    }
    else
    {
        Error(expr->op, "Unknown binary type (" + leftVal->toString() + " - " + rightVal->toString() + ")");
        return createBoolLiteral(false);
    }

    
    Error(expr->op, "Unknown binary type (" + leftVal->toString() + " - " + rightVal->toString() + ")");
    return createBoolLiteral(false);
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
                    return createIntLiteral(-literal->value->getInt());
                }
                else if (literal->value->getType() == LiteralType::FLOAT)
                {
                    return createFloatLiteral(-literal->value->getFloat());
                }
          
            
        } else if (expr->op.type==TokenType::BANG)
        {
            if (literal->value->getType() == LiteralType::BOOLEAN)
            {
                return createBoolLiteral(!literal->value->getBool());
            }
        } else if (expr->op.type==TokenType::NOT)
        {
            if (literal->value->getType() == LiteralType::BOOLEAN)
            {
                return createBoolLiteral(!literal->value->getBool());
            }
        }
        else 
        {
                Error(expr->op, "Unknown unary operator");
                return createBoolLiteral(false);
        }
    }
    else
    {
        Error(expr->op, "Unary operation on non-literal");
      return createBoolLiteral(false);;
    }

    Warning( "Failed to evaluate unary expression");
    return createBoolLiteral(false);
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

std::unique_ptr<Expr> Interpreter::visitEmptyExpr(EmptyExpr *expr)
{
    
    return std::unique_ptr<Expr>();
}


static bool literalToBool(const Literal& literal)
{
    switch (literal.getType())
    {
        case LiteralType::BOOLEAN:
            return literal.getBool();
        case LiteralType::INT:
            return literal.getInt() != 0;
        case LiteralType::FLOAT:
            return literal.getFloat() != 0.0;
        default:
            return false;
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
        return createBoolLiteral(false);
    }

    Warning( "Failed to evaluate logical expression");
   return createBoolLiteral(false);
}


std::unique_ptr<Expr> Interpreter::visitGroupingExpr(GroupingExpr *expr)
{
    return evaluate(std::move(expr->expression));
}


std::unique_ptr<Expr> Interpreter::visitLiteralExpr(LiteralExpr *expr)
{
    if (expr->value->getType() == LiteralType::INT)
    {
        return createIntLiteral(expr->value->getInt());
    } else if (expr->value->getType() == LiteralType::FLOAT)
    {
        return createFloatLiteral(expr->value->getFloat());
    } else if (expr->value->getType() == LiteralType::BOOLEAN)
    {
        return createBoolLiteral(expr->value->getBool());
    } else if (expr->value->getType() == LiteralType::STRING)
    {
        return createStringLiteral(expr->value->getString());
    } else if (expr->value->getType() == LiteralType::POINTER)
    {
       
        return createPointerLiteral(expr->value->getPointer());
    }
  
    Warning("Unknown literal type");
    return nullptr;
   
}




std::unique_ptr<Expr> Interpreter::evaluate(std::unique_ptr<Expr> expr)
{
    return expr->accept(this);
}



std::unique_ptr<Expr> Interpreter::evaluate_ref(std::unique_ptr<Expr> &expr)
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





void Interpreter::visitPrintStmt(PrintStmt *stmt)
 {
    
    auto result = evaluate(std::move(stmt->expression));

    if (!result)
    {
        return ;
    }

    if (result->getType() == ExprType::LITERAL)
    {
        LiteralExpr *literal = dynamic_cast<LiteralExpr*>(result.get());

        if (literal->value->getType() == LiteralType::INT)
        {
            std::cout << literal->value->getInt() << std::endl;
        } else if (literal->value->getType() == LiteralType::FLOAT)
        {
            std::cout << literal->value->getFloat() << std::endl;
        } else if (literal->value->getType() == LiteralType::BOOLEAN)
        {
            std::cout << literal->value->getBool() << std::endl;
        } else if (literal->value->getType() == LiteralType::STRING)
        {
            std::cout << literal->value->getString() << std::endl;
        } else if (literal->value->getType() == LiteralType::POINTER)
        {
            std::cout << literal->value->getPointer() << std::endl;
        }
    } else 
    {
       Warning("Cannot print non-literal expression");
    }


   
 }



std::unique_ptr<Expr> Interpreter::visitVariableExpr(VariableExpr *expr)
{
    std::string name = expr->name.lexeme;
    int line = expr->name.line-1;

    

    if (!this->environment->contains(name))
    {
        Warning("Load variable  '" + name + "' not  defined at line: "+std::to_string(line));
        return std::unique_ptr<EmptyExpr>();
    }
    

    Literal *value =  this->environment->get(name);
    
    if (value->getType() == LiteralType::INT)
    {
        return createIntLiteral(value->getInt());
    } else if (value->getType() == LiteralType::FLOAT)
    {
        return createFloatLiteral(value->getFloat());
    } else if (value->getType() == LiteralType::BOOLEAN)
    {
        return createBoolLiteral(value->getBool());
    } else if (value->getType() == LiteralType::STRING)
    {
        return createStringLiteral(value->getString());
    } else if (value->getType() == LiteralType::POINTER)
    {
        return createPointerLiteral(value->getPointer());
    }

    return std::unique_ptr<EmptyExpr>();
    
}

std::unique_ptr<Expr> Interpreter::visitAssignExpr(AssignExpr *expr)
{
    std::string name = expr->name.lexeme;
  

    auto value = evaluate(std::move(expr->value));

    if (!value || !this->environment->contains(name))
    {
        Error(expr->name,"Can Assign, variable  '" + name + "' at line: "+std::to_string(expr->name.line));
        return std::unique_ptr<EmptyExpr>();
    }
       
     if (value->getType() == ExprType::LITERAL)
     {
         LiteralExpr *literal = dynamic_cast<LiteralExpr*>(value.get());
         Literal *lAssign = this->environment->get(name);
   
        if (literal->value->getType() == LiteralType::INT)
        {
           
            if (lAssign->getType() != LiteralType::INT)
            {
                Warning("Cannot assign to non-int variable");
                return value;
            }
            lAssign->setInt(literal->value->getInt());

           
        } else if (literal->value->getType() == LiteralType::FLOAT)
        {
            if (lAssign->getType() != LiteralType::FLOAT)
            {
                Warning("Cannot assign to non-float variable");
                return value;
            }
            lAssign->setFloat(literal->value->getFloat());
          
        } else if (literal->value->getType() == LiteralType::BOOLEAN)
        {
            if (lAssign->getType() != LiteralType::BOOLEAN)
            {
                Warning("Cannot assign to non-boolean variable");
                 return value;
            }
            lAssign->setBool(literal->value->getBool());
           
        } else if (literal->value->getType() == LiteralType::STRING)
        {
            if (lAssign->getType() != LiteralType::STRING)
            {
                Warning("Cannot assign to non-string variable");
                return value;
            }
            lAssign->setString(literal->value->getString());
          
        } else if (literal->value->getType() == LiteralType::POINTER)
        {
            if (lAssign->getType() != LiteralType::POINTER)
            {
                Warning("Cannot assign to non-pointer variable");
                return value;
            }
            lAssign->setPointer(literal->value->getPointer());
         
        }
    } else 
    {
       Warning("Cannot define non-literal expression");
    }

    return value;
}

void Interpreter::visitVarStmt(VarStmt *stmt)
 {
   

     auto value = evaluate_ref(stmt->initializer);
     if (!value)
     {
        for (auto& name : stmt->names)
        {
            Warning("Can Assign, variable  '" + name.lexeme + "'");
        }
        return;
     }
        
     
     if (value->getType() == ExprType::LITERAL)
     {
         LiteralExpr *literal = dynamic_cast<LiteralExpr*>(value.get());

         for (auto& token : stmt->names)
         {
             std::string name = token.lexeme;
            
        



        
         if (literal->value->getType() == LiteralType::INT)
         {
             Literal *newLiteral = new Literal(literal->value->getInt());
             if (!this->environment->define(name,  newLiteral))
             {
                 Warning("Variable '"+name+"' already defined at line: "+std::to_string(token.line));
                 delete newLiteral;
             }

         } else if (literal->value->getType() == LiteralType::FLOAT)
         {
             Literal *newLiteral = new Literal(literal->value->getFloat());
             if (!this->environment->define(name,  newLiteral))
             {
                 Warning("Variable '"+name+"' already defined at line: "+std::to_string(token.line));
                 delete newLiteral;
             }
             
         } else if (literal->value->getType() == LiteralType::BOOLEAN)
         {
             Literal *newLiteral = new Literal(literal->value->getBool());
             if (!this->environment->define(name,  newLiteral))
             {
                 Warning("Variable '"+name+"' already defined at line: "+std::to_string(token.line));
                 delete newLiteral;
             }
             
         } else if (literal->value->getType() == LiteralType::STRING)
         {
             Literal *newLiteral = new Literal(literal->value->getString());
             if (!this->environment->define(name,  newLiteral))
             {
                Warning("Variable '"+name+"' already defined at line: "+std::to_string(token.line));
                 delete newLiteral;
             }
             
         } else if (literal->value->getType() == LiteralType::POINTER)
         {
             Literal *newLiteral = new Literal(literal->value->getPointer());
             if (!this->environment->define(name,  newLiteral))
             {
                 Warning("Variable '"+name+"' already defined at line: "+std::to_string(token.line));
                 delete newLiteral;
             }
         }
        }

     } else 
     {
        Warning("Cannot define non-literal expression");
     }
 }



 void Interpreter::executeBlock(BlockStmt *stmt, Environment *env)
{
    auto previous = this->environment;
    this->environment = env;
    for (auto& stmt : stmt->declarations)
    {
        execute(stmt);
    }
    this->environment = previous;
}

void Interpreter::visitBlockStmt(BlockStmt *stmt)
 {
    this->currentDepth++;
   // Environment *newEnv = new Environment(this->currentDepth, this->environment);
     std::shared_ptr<Environment> newEnv = std::make_shared<Environment>(this->currentDepth,this->environment);
   

    executeBlock(stmt, newEnv.get());
    this->currentDepth--;
    
 }

void Interpreter::visitExpressionStmt(ExpressionStmt *stmt)
 {
     evaluate(std::move(stmt->expression));    
 }

void Interpreter::visitProgram(Program *stmt)
{
    for (auto& stmt : stmt->statements)
    {
        execute(stmt);
    }   
    execute(stmt->statement);
     
}

void Interpreter::visitEmptyStmt(EmptyStmt *stmt)
{

    Info("Interpreting empty statement");
}

void Interpreter::visitCallStmt(CallStmt *stmt)
{

    Info("TODO: Interpreting call statement");
}

void Interpreter::visitProcedureCallStmt(ProcedureCallStmt *stmt)
{
    std::string name = stmt->name.lexeme;

    if (procedureList.find(name) == procedureList.end())
    {
        Warning("Procedure '"+name+"' not defined at line: "+std::to_string(stmt->name.line));
        return;
    }
    ProcedureStmt *procedure = procedureList[name];
    
    unsigned int numArgsExpectd = procedure->parameter.size();
    unsigned int numArgs = stmt->arguments.size();

    if (numArgs != numArgsExpectd)
    {
        Warning("Incorrect number of arguments passed to procedure '"+name+"' at line: "+std::to_string(stmt->name.line)+ " expected: "+std::to_string(numArgsExpectd)+" got: "+std::to_string( numArgs));
        return;
    }
    this->currentDepth++;
   // Environment *newEnv = new Environment(this->currentDepth, this->environment);

    std::shared_ptr<Environment> newEnv = std::make_shared<Environment>(this->currentDepth,this->environment);
   

    

    for (unsigned int i = 0; i < numArgs; i++)
    {
        std::unique_ptr<Expr> value =   evaluate(std::move(stmt->arguments[i]));
        if (!value) 
        {
            Warning("Invalid argument passed to procedure '"+name+"' at line: "+std::to_string(stmt->name.line));
            return ;
        };
        std::string argName = procedure->parameter[i].get()->name;
        if (value->getType() == ExprType::LITERAL)
        {
            LiteralExpr *literal = dynamic_cast<LiteralExpr*>(value.get());
            if (literal->value->getType() == LiteralType::INT)
            {
                int value = literal->value->getInt();
                newEnv->define(argName, new Literal(value));
            } else if (literal->value->getType() == LiteralType::FLOAT)
            {
                float value = literal->value->getFloat();
                newEnv->define(argName, new Literal(value));
            } else if (literal->value->getType() == LiteralType::BOOLEAN)
            {
                bool value = literal->value->getBool();
                newEnv->define(argName, new Literal(value));
               
            } else if (literal->value->getType() == LiteralType::STRING)
            {
                std::string value = literal->value->getString();
                newEnv->define(argName, new Literal(value));
            } else if (literal->value->getType() == LiteralType::POINTER)
            {
                newEnv->define(argName, new Literal(literal->value->getPointer()));
            }
              
        }
    }

   

    auto previous = this->environment;
    this->environment = newEnv.get();
    this->execute(procedure->body);
    this->environment = previous;

    this->currentDepth--;
    

    

  
}

void Interpreter::visitProcedureStmt(ProcedureStmt *stmt)
{
    if (procedureList.find(stmt->name) != procedureList.end())
    {
        Warning("Procedure '"+stmt->name+"' already defined ");
        return;
    }
 //   Info("TODO: CREATE procedure : "+stmt->name);
    procedureList[stmt->name] = stmt;
}

//********************************************************************************** */


void Interpreter::Error(const Token &token,const std::string &message)
{
    panicMode = true;
    int line = token.line;
    std::string text ="ERROR: " +message+ " at line: " +std::to_string(line);
    std::cout<<text<<std::endl;
}

void Interpreter::Warning(const std::string &message)
{
    
    std::string text ="WARNING: " +message;
    std::cout<<text<<std::endl;
}

void Interpreter::Info(const std::string &message)
{
   std::string text ="INFO: " +message;
    std::cout<<text<<std::endl;
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



//*****************************************************************************************

Environment::Environment(int depth,Environment *parent):m_depth(depth),m_parent(parent)
{
   // std::cout<<"Create Environment: "<< m_depth << std::endl;
}

Environment::~Environment()
{

  //  std::cout<<"Delete Environment: "<< m_depth << std::endl;
    for (auto it = m_values.begin(); it != m_values.end(); ++it)
    {
       // std::cout<<"delete Variable: "<< it->first << " = " << it->second->toString() << std::endl;
       delete it->second;
    }
}

bool Environment::define(const std::string &name, Literal *value)
{
    if (m_values.find(name) != m_values.end())
    {
        return false;
    }
    m_values[name] = value;
    return true;    
}

Literal *Environment::get(const std::string &name)
{
    
    if (m_values.find(name) != m_values.end())
    {
        return m_values[name];
    }
    if (m_parent!=nullptr)
    {
        if (m_parent->contains(name))
        {
            return m_parent->get(name);
        }
    }

   return nullptr;

}

bool Environment::assign(const std::string &name, Literal *value)
{
    if (contains(name))
    {
        m_values[name] = value;
        return true;
    }
    if (m_parent!=nullptr)
    {
        if (m_parent->contains(name))
        {
            return m_parent->assign(name, value);
        }
    }

   return false;
}

bool Environment::contains(const std::string &name)
{  
     if (m_values.find(name) != m_values.end())
    {
        return true;
    }
    if (m_parent!=nullptr)
    {
        if (m_parent->contains(name))
        {
            return true;
        }
    }

   return false;
}
