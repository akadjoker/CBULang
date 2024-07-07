
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

        if (expr->value->getType() == LiteralType::NUMBER)
        {
            return createNumberLiteral(expr->value->getNumber());
        }
        else if (expr->value->getType() == LiteralType::STRING)
        {
            return createStringLiteral(expr->value->getString());
        }
        else if (expr->value->getType() == LiteralType::BOOLEAN)
        {
            return createBoolLiteral(expr->value->getBool());
        }
        else
        {
            return std::make_shared<EmptyExpr>();
        }

   
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
           
            if (leftLiteral->value->getType() == LiteralType::NUMBER && rightLiteral->value->getType() == LiteralType::NUMBER)
            {
                return createNumberLiteral(leftLiteral->value->getNumber() + rightLiteral->value->getNumber());
            }
             else if (leftLiteral->value->getType() == LiteralType::STRING && rightLiteral->value->getType() == LiteralType::STRING)
            {
                std::string value = std::string(leftLiteral->value->getString());
                value += std::string(rightLiteral->value->getString());
                return createStringLiteral(value);
            } 
             else if (leftLiteral->value->getType() == LiteralType::STRING && rightLiteral->value->getType() == LiteralType::NUMBER)
            {
                std::string value = leftLiteral->value->getString() + std::to_string(rightLiteral->value->getNumber());
                return createStringLiteral(value);
            } else if (leftLiteral->value->getType() == LiteralType::STRING && rightLiteral->value->getType() == LiteralType::BOOLEAN)
            {
                std::string value = leftLiteral->value->getString() + std::to_string(rightLiteral->value->getBool());
                return createStringLiteral(value);
            } else 
            {
                                
                  Error(expr->op, "Unsupported operation (" + leftVal->toString() + " add " + rightVal->toString() + ")");
                  return createBoolLiteral(false);
                
            }
        }
        else if (expr->op.type == TokenType::MINUS) // - subtraction
        {
            if (leftLiteral->value->getType() == LiteralType::NUMBER && rightLiteral->value->getType() == LiteralType::NUMBER)
            {
                return createNumberLiteral(leftLiteral->value->getNumber() - rightLiteral->value->getNumber());
            }
            else 
            {
                                
                  Error(expr->op, "Unsupported operation (" + leftVal->toString() + " sub " + rightVal->toString() + ")");
                  return createBoolLiteral(false);
                
            }
        }
        else if (expr->op.type == TokenType::STAR)// * multiplication
        {
             if (leftLiteral->value->getType() == LiteralType::NUMBER && rightLiteral->value->getType() == LiteralType::NUMBER)
            {
                return createNumberLiteral(leftLiteral->value->getNumber() * rightLiteral->value->getNumber());
            }
            else 
            {
                                
                  Error(expr->op, "Unsupported operation (" + leftVal->toString() + " mult " + rightVal->toString() + ")");
                  return createBoolLiteral(false);
                
            }
        }
        else if (expr->op.type == TokenType::SLASH)// / division
        {
            if (leftLiteral->value->getType() == LiteralType::NUMBER && rightLiteral->value->getType() == LiteralType::NUMBER)
            {
                if (rightLiteral->value->getNumber() == 0)
                {
                    Error(expr->op, "Division by zero");
                    return createNumberLiteral(0);
                }
                return createNumberLiteral(leftLiteral->value->getNumber() / rightLiteral->value->getNumber());
            }else
            {
                Error(expr->op, "Unsupported operation (" + leftVal->toString() + " div " + rightVal->toString() + ")");
                return createBoolLiteral(false);
            }
        }else if (expr->op.type == TokenType::MOD)
        {
             if (leftLiteral->value->getType() == LiteralType::NUMBER && rightLiteral->value->getType() == LiteralType::NUMBER)
            {
                return createNumberLiteral(fmod(leftLiteral->value->getNumber(), rightLiteral->value->getNumber()));
            }else 
            {
                Error(expr->op, "Unsupported operation (" + leftVal->toString() + " mod " + rightVal->toString() + ")");
                return createBoolLiteral(false);
            }
        }else if (expr->op.type == TokenType::POWER)
        {
            if (leftLiteral->value->getType() == LiteralType::NUMBER && rightLiteral->value->getType() == LiteralType::NUMBER)
            {
                return createNumberLiteral(std::pow(leftLiteral->value->getNumber(), rightLiteral->value->getNumber()));
            }
            else
            {
                Error(expr->op, "Unsupported operation (" + leftVal->toString() + " pow " + rightVal->toString() + ")");
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
            else if (leftLiteral->value->getType() == LiteralType::NUMBER && rightLiteral->value->getType() == LiteralType::NUMBER)
            {
                return createBoolLiteral(leftLiteral->value->getNumber() == rightLiteral->value->getNumber());
            }
            else
            {
                                
                  Error(expr->op, "Unsupported operation (" + leftVal->toString() + " equal " + rightVal->toString() + ")");
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
            } if (leftLiteral->value->getType() == LiteralType::NUMBER && rightLiteral->value->getType() == LiteralType::NUMBER)
            {
                return createBoolLiteral(leftLiteral->value->getNumber() != rightLiteral->value->getNumber());
            }
            else
            {
                Error(expr->op, "Unsupported operation (" + leftVal->toString() + " not equal " + rightVal->toString() + ")");
                return createBoolLiteral(false);
            }
        }else if (expr->op.type == TokenType::LESS)// <
        {
            if (leftLiteral->value->getType() == LiteralType::NUMBER && rightLiteral->value->getType() == LiteralType::NUMBER)
            {
                return createBoolLiteral(leftLiteral->value->getNumber() < rightLiteral->value->getNumber());
            }  else 
            if (leftLiteral->value->getType() == LiteralType::STRING && rightLiteral->value->getType() == LiteralType::STRING)
            {
                return createBoolLiteral(leftLiteral->value->getString() < rightLiteral->value->getString());
            }
            else
            {
               Error(expr->op, "Unsupported operation (" + leftVal->toString() + " less " + rightVal->toString() + ")");
               return createBoolLiteral(false);
            }
         }else if (expr->op.type == TokenType::LESS_EQUAL)// <=
        {
            if (leftLiteral->value->getType() == LiteralType::NUMBER && rightLiteral->value->getType() == LiteralType::NUMBER)
            {
                return createBoolLiteral(leftLiteral->value->getNumber() <= rightLiteral->value->getNumber());
            } else 
            if (leftLiteral->value->getType() == LiteralType::STRING && rightLiteral->value->getType() == LiteralType::STRING)
            {
                return createBoolLiteral(leftLiteral->value->getString() <= rightLiteral->value->getString());
            }
            else
            {
               Error(expr->op, "Unsupported operation (" + leftVal->toString() + " less equal " + rightVal->toString() + ")");
                return createBoolLiteral(false);
            }
         }else if (expr->op.type == TokenType::GREATER)// >
        {
             if (leftLiteral->value->getType() == LiteralType::NUMBER && rightLiteral->value->getType() == LiteralType::NUMBER)
            {
                return createBoolLiteral(leftLiteral->value->getNumber() > rightLiteral->value->getNumber());
            } else if (leftLiteral->value->getType() == LiteralType::STRING && rightLiteral->value->getType() == LiteralType::STRING)
            {
                return createBoolLiteral(leftLiteral->value->getString() > rightLiteral->value->getString());
            }
            else
            {
                Error(expr->op, "Unsupported operation (" + leftVal->toString() + " greater " + rightVal->toString() + ")");
                return createBoolLiteral(false);
            }
         }else if (expr->op.type == TokenType::GREATER_EQUAL)// >=
        {
            if (leftLiteral->value->getType() == LiteralType::NUMBER && rightLiteral->value->getType() == LiteralType::NUMBER)
            {
                return createBoolLiteral(leftLiteral->value->getNumber() >= rightLiteral->value->getNumber());
            } else if (leftLiteral->value->getType() == LiteralType::STRING && rightLiteral->value->getType() == LiteralType::STRING)
            {
                return createBoolLiteral(leftLiteral->value->getString() >= rightLiteral->value->getString());
            } 
            else
            {
               Error(expr->op, "Unsupported operation (" + leftVal->toString() + " greater equal " + rightVal->toString() + ")");
                return createBoolLiteral(false);
            }
        } else if (expr->op.type == TokenType::PLUS_EQUAL)// +=
        {
            if (leftLiteral->value->getType() == LiteralType::NUMBER && rightLiteral->value->getType() == LiteralType::NUMBER)
            {
                return createNumberLiteral(leftLiteral->value->getNumber() + rightLiteral->value->getNumber());
            } else
            {
                Error(expr->op, "Unsupported operation (" + leftVal->toString() + " plus equal " + rightVal->toString() + ")");
                return createBoolLiteral(false);
            }
        } else if (expr->op.type == TokenType::MINUS_EQUAL)// -=
        {
            if (leftLiteral->value->getType() == LiteralType::NUMBER && rightLiteral->value->getType() == LiteralType::NUMBER)
            {
                return createNumberLiteral(leftLiteral->value->getNumber() - rightLiteral->value->getNumber());
            } else
            {
                Error(expr->op, "Unsupported operation (" + leftVal->toString() + " minus equal " + rightVal->toString() + ")");
                return createBoolLiteral(false);
            }
        } else if (expr->op.type == TokenType::STAR_EQUAL)// *=
        {
            if (leftLiteral->value->getType() == LiteralType::NUMBER && rightLiteral->value->getType() == LiteralType::NUMBER)
            {
                return createNumberLiteral(leftLiteral->value->getNumber() * rightLiteral->value->getNumber());
            } else
            {
                Error(expr->op, "Unsupported operation (" + leftVal->toString() + " star equal " + rightVal->toString() + ")");
                return createBoolLiteral(false);
            }
        } else if (expr->op.type == TokenType::SLASH_EQUAL)// /=
        {
            if (leftLiteral->value->getType() == LiteralType::NUMBER && rightLiteral->value->getType() == LiteralType::NUMBER)
            {
                if (rightLiteral->value->getNumber() == 0)
                {
                    Error(expr->op, "Division by zero");
                    return createNumberLiteral(leftLiteral->value->getNumber());
                }
                return createNumberLiteral(leftLiteral->value->getNumber() / rightLiteral->value->getNumber());
            } else
            {
                Error(expr->op, "Unsupported operation (" + leftVal->toString() + " slash equal " + rightVal->toString() + ")");
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
            
                if (literal->value->getType() == LiteralType::NUMBER)
                {
                    return createNumberLiteral(-literal->value->getNumber());
                } 
            
        } else if (expr->op.type==TokenType::BANG)
        {
            if (literal->value->getType() == LiteralType::BOOLEAN)
            {
                return createBoolLiteral(!literal->value->getBool());
            } else 
            if (literal->value->getType() == LiteralType::NUMBER)
            {
                 bool value= (literal->value->getNumber() == 0.0) ? 1 : 0;
                return createBoolLiteral(!value);
            }
        } else if (expr->op.type==TokenType::NOT)
        {
            if (literal->value->getType() == LiteralType::BOOLEAN)
            {
                return createBoolLiteral(!literal->value->getBool());
            } else 
            if (literal->value->getType() == LiteralType::NUMBER)
            {
                bool value= (literal->value->getNumber() == 0.0) ? 1 : 0;
                return createBoolLiteral(!value);
            }
        } else if (expr->op.type==TokenType::INC)
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

                if (realLiteral->getType() == LiteralType::NUMBER)
                {
                    if (isPrefix)
                    {
                        double value = realLiteral->getNumber();
                        value++;
                        realLiteral->setNumber(value);
                        return createNumberLiteral(value);
                    }
                    else
                    {
                        double value = realLiteral->getNumber();
                        realLiteral->setNumber(value + 1.0);
                        return createNumberLiteral(value);
                    }
                }
                else if (realLiteral->getType() == LiteralType::NUMBER)
                {
                     if (isPrefix)
                     {
                        double value = realLiteral->getNumber();
                        value++;
                        realLiteral->setNumber(value);
                        return createNumberLiteral(value);

                     } else 
                     {
                        double value = realLiteral->getNumber();
                        realLiteral->setNumber(value + 1.0);
                        return createNumberLiteral(value);
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

                if (realLiteral->getType() == LiteralType::NUMBER)
                {
                    if (isPrefix)
                    {
                        double value = realLiteral->getNumber();
                        value--;
                        realLiteral->setNumber(value);
                        return createNumberLiteral(value);

                    } else 
                    {
                        double value = realLiteral->getNumber();
                        realLiteral->setNumber(value - 1.0);
                        return createNumberLiteral(value);
                    }
                }
                else if (realLiteral->getType() == LiteralType::NUMBER)
                {
                    if (isPrefix)
                    {
                        double value = realLiteral->getNumber();
                        value--;
                        realLiteral->setNumber(value);
                        return createNumberLiteral(value);
                    } else
                    {
                        double value = realLiteral->getNumber();
                        realLiteral->setNumber(value - 1.0);
                        return createNumberLiteral(value);
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
