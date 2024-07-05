
std::shared_ptr<Expr> Interpreter::visitLogicalExpr(LogicalExpr *expr)
{
    auto leftVal = evaluate(expr->left);
    auto rightVal = evaluate(expr->right);

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


std::shared_ptr<Expr> Interpreter::visitGroupingExpr(GroupingExpr *expr)
{
    return evaluate(expr->expression);
}


std::shared_ptr<Expr> Interpreter::visitLiteralExpr(LiteralExpr *expr)
{

        return std::shared_ptr<Expr>(expr, [](Expr*){});  

   
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




std::shared_ptr<Expr> Interpreter::visitUnaryExpr(UnaryExpr *expr)
{
    auto rightVal = evaluate(expr->right);
    bool isPrefix = expr->isPrefix;

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
        } else if (expr->op.type==TokenType::INC)
        {
                // if (expr->right->getType() == ExprType::VARIABLE)
                // {
                //     Error(expr->op, "Only variables can be incremented"+expr->right->toString());
                //     return std::shared_ptr<Expr>(expr, [](Expr*){});  
                // }

                VariableExpr *variable = static_cast<VariableExpr*>(expr->right.get());
                

                std::string name = variable->name.lexeme;

                std::shared_ptr<Literal> oldVariable = this->environment->get(name);
                if (!oldVariable)
                {
                    Error(expr->op, "Can Increment, variable  '" + name + "' ");
                    return std::shared_ptr<Expr>(expr, [](Expr*){});  
                }
                

                Literal *realLiteral = static_cast<Literal*>(oldVariable.get());

                if (realLiteral->getType() == LiteralType::INT)
                {
                    if (isPrefix)
                    {
                        int value = realLiteral->getInt();
                        value++;
                        realLiteral->setInt(value);
                        return createIntLiteral(value);
                    }
                    else
                    {
                        int value = realLiteral->getInt();
                        realLiteral->setInt(value + 1);
                        return createIntLiteral(value);
                    }
                }
                else if (realLiteral->getType() == LiteralType::FLOAT)
                {
                     if (isPrefix)
                     {
                        double value = realLiteral->getFloat();
                        value++;
                        realLiteral->setFloat(value);
                        return createFloatLiteral(value);

                     } else 
                     {
                        double value = realLiteral->getFloat();
                        realLiteral->setFloat(value + 1.0);
                        return createFloatLiteral(value);
                     }
                    
                }

              return std::shared_ptr<Expr>(expr, [](Expr*){});  

              
            
        } else if (expr->op.type==TokenType::DEC)
        {
            VariableExpr *variable = static_cast<VariableExpr*>(expr->right.get());

                std::string name = variable->name.lexeme;

                std::shared_ptr<Literal> oldVariable = this->environment->get(name);
                if (!oldVariable)
                {
                    Error(expr->op, "Can Increment, variable  '" + name + "' ");
                    return std::shared_ptr<Expr>(expr, [](Expr*){});  
                }
                

                Literal *realLiteral = static_cast<Literal*>(oldVariable.get());

                if (realLiteral->getType() == LiteralType::INT)
                {
                    if (isPrefix)
                    {
                        int value = realLiteral->getInt();
                        value--;
                        realLiteral->setInt(value);
                        return createIntLiteral(value);

                    } else 
                    {
                        int value = realLiteral->getInt();
                        realLiteral->setInt(value - 1);
                        return createIntLiteral(value);
                    }
                }
                else if (realLiteral->getType() == LiteralType::FLOAT)
                {
                    if (isPrefix)
                    {
                        double value = realLiteral->getFloat();
                        value--;
                        realLiteral->setFloat(value);
                        return createFloatLiteral(value);
                    } else
                    {
                        double value = realLiteral->getFloat();
                        realLiteral->setFloat(value - 1.0);
                    }
                }

              return std::shared_ptr<Expr>(expr, [](Expr*){});  

    
        }
        else 
        {
                Error(expr->op, "Unknown unary operator");
                return std::shared_ptr<Expr>(expr, [](Expr*){});  
        }
    }
    else
    {
        Error(expr->op, "Unary operation on non-literal");
        return std::shared_ptr<Expr>(expr, [](Expr*){});  
    }

    Warning( "Failed to evaluate unary expression");
      return std::shared_ptr<Expr>(expr, [](Expr*){});  
}
