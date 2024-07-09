#include "pch.h"
#include "Interpreter.hpp"
#include "Literal.hpp"
#include "Utils.hpp"

unsigned long processID = 0;

static uintptr_t getAddress(const void *ptr)
{
    return reinterpret_cast<uintptr_t>(ptr);
}

Interpreter::Interpreter()
{
    Info("Create Interpreter");
    currentDepth = 0;
    addressLoop = 0x0;
    environment = std::make_shared<Environment>(0, nullptr);
    BlockID = 0;
    context = std::make_shared<ExecutionContext>(this);
    panicMode = false;
    start_time = std::chrono::high_resolution_clock::now();
    time_elapsed();
}

Interpreter::~Interpreter()
{
    Log(0, "Release Interpreter");

    processes.clear();
    procedureList.clear();
    functionList.clear();
    nativeFunctions.clear();
}

double Interpreter::time_elapsed()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now - start_time;
    auto milliseconds = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(duration).count();
    return milliseconds;

    // auto now = std::chrono::system_clock::now();
    // auto duration = now - start_time;//now.time_since_epoch();
    // auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    // return seconds;
}

std::shared_ptr<Expr> Interpreter::visit( std::shared_ptr<Expr> expr)
{
      if (!expr)
    {
        Warning("Evaluate null expression");
        return std::make_shared<EmptyExpr>();
    }

    std::shared_ptr<Expr> result = expr->accept(this);

        expr.reset();
  //  Info("Result: " + std::to_string(expr.use_count()) + " " + expr->toString());
    return result;
}

std::shared_ptr<Expr> Interpreter::evaluate( std::shared_ptr<Expr> expr)
{
    if (!expr)
    {
        Warning("Evaluate null expression");
        return std::make_shared<EmptyExpr>();
    }
    std::shared_ptr<Expr> result = expr->accept(this);

    expr.reset();

 //   Info("Result: " + std::to_string(expr.use_count()) + " " + expr->toString());

    return result;
}


#include "InterpreterExp.cc"

std::shared_ptr<Expr> Interpreter::visitNowExpr(NowExpr *expr)
{
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::duration<double>>(duration).count();

    return Factory::Instance().createFloatLiteral(seconds);
}

std::shared_ptr<Expr> Interpreter::visitEmptyExpr(EmptyExpr *expr)
{
    return std::make_shared<EmptyExpr>();
}



void Interpreter::execute(Stmt *statement)
{
    if (statement == nullptr)
    {
        Error("execute null statement");
        return;
    }
    statement->accept(this);
}

void Interpreter::build(std::shared_ptr<Stmt> program)
{
       execute(program);

  }

void Interpreter::execute(const std::shared_ptr<Stmt> &stmt)
{
    if (stmt == nullptr)
        return;
   // std::cout << "execute: " << stmt->toString() << std::endl;
    stmt->accept(this);

}

void Interpreter::visitBlockStmt(BlockStmt *stmt)
{
    this->currentDepth++;
//    std::shared_ptr<Environment> newEnv = std::make_shared<Environment>(this->currentDepth, this->environment.get());
      executeBlock(stmt,   this->environment);
  //  this->currentDepth--;
}

void Interpreter::executeBlock(BlockStmt *stmt, const std::shared_ptr<Environment> &env)
{
    auto previous = this->environment;
    this->environment = env;
    for (const auto &stmt : stmt->declarations)
    {
        try
        {
            execute(stmt);
        }
        catch (ReturnException &returnValue)
        {
            break;
        }
    }
    this->environment = previous;
}

bool Interpreter::run()
{


    for (size_t i = 0; i < processes.size(); i++)
    {
      
        
        if (!processes[i]->running())
        {
            remove_processes.push_back(std::move(processes[i]));
            processes.erase(processes.begin() + i);
            i--;
            continue;
        }


        auto previus = environment;
        environment = processes[i]->environment;

        processes[i]->run();

        environment = previus;
        

      //  environment = environmentStack.top();
       //environmentStack.pop();
    }
    
    remove_processes.clear();
    


   

    
    return true;
}

void Interpreter::visitPrintStmt(PrintStmt *stmt)
{

    auto result = evaluate(stmt->expression);

    if (!result)
    {
        return;
    }

    if (result->getType() == ExprType::LITERAL)
    {
        LiteralExpr *literal = dynamic_cast<LiteralExpr *>(result.get());

        literal->Value()->print();
    }
    else
    {
        Warning("Cannot print non-literal expression");
    }
}

std::shared_ptr<Expr> Interpreter::visitVariableExpr(VariableExpr *expr)
{
    std::string name = expr->name.lexeme;
    int line = expr->name.line - 1;

    if (!this->environment->contains(name))
    {
        Error("Load variable  '" + name + "' not  defined at line: " + std::to_string(line));
        return std::make_shared<EmptyExpr>();
    }

    Literal *value =this->environment->get(name);
    if (!value)
    {
        Error("Load variable  '" + name + "' is null at line: " + std::to_string(line));
        return std::make_shared<EmptyExpr>();
    }
    
    if (value->getType() == LiteralType::INT)
    {
        return Factory::Instance().createIntegerLiteral(value->getInt());
    }
    else if (value->getType() == LiteralType::BOOLEAN)
    {
        return Factory::Instance().createBoolLiteral(value->getBool());
    }
    else if (value->getType() == LiteralType::STRING)
    {
        return Factory::Instance().createStringLiteral(value->getString());
    } else if (value->getType() == LiteralType::FLOAT)
    {
        return Factory::Instance().createFloatLiteral(value->getFloat());
    }
    else 
    {
        Error("Load variable  '" + name + "' is null at line: " + std::to_string(line)+" type:"+ value->toString());
        return std::make_shared<EmptyExpr>();
    }


    return std::make_shared<EmptyExpr>();
}

std::shared_ptr<Expr> Interpreter::visitAssignExpr(AssignExpr *expr)
{
    std::string name = expr->name.lexeme;

    auto value = evaluate(expr->value);
    Literal *oldLiteral = this->environment->get(name);

    if (!value || !oldLiteral)
    {
        Error(expr->name, "Can Assign, variable  '" + name + "' ");
        return std::make_shared<EmptyExpr>();
    }

    

    if (value->getType() == ExprType::LITERAL)
    {

        LiteralExpr *literal = dynamic_cast<LiteralExpr *>(value.get());
        if (!literal)
        {
            Error(expr->name, "Can Assign, variable  '" + name + "' ");
            return std::make_shared<EmptyExpr>();
        }
        

      
     
        if (!this->environment->assign(name, literal->value))
        {
            Error(expr->name, "Assign variable  '" + name + "'" );
            return std::make_shared<EmptyExpr>();
        }

      
    }
    else
    {
        Warning("Cannot assign non-literal expression");
    }

    return value;
}

void Interpreter::visitVarStmt(VarStmt *stmt) /// define variables
{

    auto value = evaluate(stmt->initializer);
    if (!value)
    {
        for (auto &name : stmt->names)
        {
            Warning("Can define, variable  '" + name.lexeme + "'");
        }
        return;
    }

    if (value->getType() == ExprType::LITERAL)
    {
        LiteralExpr *literal = dynamic_cast<LiteralExpr *>(value.get());

        for (auto &token : stmt->names)
        {
            std::string name = token.lexeme;
        

            if (!this->environment->define(name, literal->value))
            {
                Warning("Variable '" + name + "' already defined at line: " + std::to_string(token.line));
            } 
        }
    }
    else
    {
        Warning("Cannot define non-literal expression");
    }
}



void Interpreter::visitExpressionStmt(ExpressionStmt *stmt)
{
    evaluate(stmt->expression);
}

void Interpreter::visitProgram(Program *stmt)
{
     


    for (auto &stmt : stmt->statements)
    {
        try
        {
            execute(stmt);
        }
        catch (ReturnException &returnValue)
        {
            break;
        }
    }
    try
    {
        execute(stmt->statement);
        
    }
    catch (ReturnException &returnValue)
    {
    }
}

void Interpreter::visitEmptyStmt(EmptyStmt *stmt)
{

    Info("Interpreting empty statement");
}

void Interpreter::visitProcedureStmt(ProcedureStmt *stmt)
{
    if (procedureList.find(stmt->name) != procedureList.end())
    {
        Error("Procedure '" + stmt->name + "' already defined ");
        return;
    }

    procedureList[stmt->name] = stmt;
}

void Interpreter::visitProcedureCallStmt(ProcedureCallStmt *stmt)
{
    std::string name = stmt->name.lexeme;

    if (procedureList.find(name) == procedureList.end())
    {
        Error("Procedure '" + name + "' not defined at line: " + std::to_string(stmt->name.line));
        return;
    }

    ProcedureStmt *procedure = procedureList[name];
    if (!procedure)
    {
        Error("Procedure '" + name + "' die");
        return;
    }

    unsigned int numArgsExpectd = procedure->parameter.size();
    unsigned int numArgs = stmt->arguments.size();

    if (numArgs != numArgsExpectd)
    {
        Error("Incorrect number of arguments passed to procedure '" + name + "' at line: " + std::to_string(stmt->name.line) + " expected: " + std::to_string(numArgsExpectd) + " got: " + std::to_string(numArgs));
        return;
    }
    this->currentDepth++;
    std::shared_ptr<Environment> newEnv = std::make_shared<Environment>(this->currentDepth, this->environment.get());

    for (unsigned int i = 0; i < numArgs; i++)
    {

        std::shared_ptr<Expr> value = evaluate(stmt->arguments[i]);
        if (!value)
        {
            Error("Invalid argument passed to procedure '" + name + "' at line: " + std::to_string(stmt->name.line));
            return;
        };
        std::string argName = procedure->parameter[i].get()->name;

        if (value->getType() == ExprType::LITERAL)
        {
            LiteralExpr *literal = dynamic_cast<LiteralExpr *>(value.get());
            if (!newEnv->define(argName, literal->value))
            {
                Error("Variable '" + argName + "' already defined at line: " + std::to_string(stmt->name.line));
                return;
            }
        }
    }

    auto previous = this->environment;
    this->environment = newEnv;
    this->execute(procedure->body);
    this->environment = previous;
    this->currentDepth--;
}

std::shared_ptr<Expr> Interpreter::visitFunctionCallExpr(FunctionCallExpr *expr)
{

    std::string name = expr->name.lexeme;

    if (functionList.find(name) == functionList.end())
    {
        Error("Function '" + name + "' not defined at line: " + std::to_string(expr->name.line));
        return std::make_shared<EmptyExpr>();
    }
    FunctionStmt *function = functionList[name];
    if (!function)
    {
        Error("Function '" + name + "' die");
        return std::make_shared<EmptyExpr>();
    }

    unsigned int numArgsExpectd = function->parameter.size();
    unsigned int numArgs = expr->arguments.size();

    if (numArgs != numArgsExpectd)
    {
        Error("Incorrect number of arguments passed to function '" + name + "' at line: " + std::to_string(expr->name.line) + " expected: " + std::to_string(numArgsExpectd) + " got: " + std::to_string(numArgs));
        return std::make_shared<EmptyExpr>();
    }
    this->currentDepth++;
    std::shared_ptr<Environment> newEnv = std::make_shared<Environment>(this->currentDepth, this->environment.get());
    for (unsigned int i = 0; i < numArgs; i++)
    {
        std::shared_ptr<Expr> value = evaluate(expr->arguments[i]);
        if (!value)
        {
            Error(expr->name, "Invalid argument passed to function '" + name + "' at line: " + std::to_string(expr->name.line));
            return std::make_shared<EmptyExpr>();
        };
        std::string argName = function->parameter[i].get()->name;
        if (value->getType() == ExprType::LITERAL)
        {
            LiteralExpr *literal = dynamic_cast<LiteralExpr *>(value.get());
            if (!newEnv->define(argName, literal->value))
            {
                Error(expr->name, "Variable '" + argName + "' already defined at line: " + std::to_string(expr->name.line));
                return std::make_shared<EmptyExpr>();
            }
        }
    }

    auto previous = this->environment;
    this->environment = newEnv;
    std::shared_ptr<Expr> result = nullptr;
    BlockStmt *block = dynamic_cast<BlockStmt *>(function->body.get());
    try
    {
        for (auto stmt : block->declarations)
        {
            execute(stmt);
        }
    }
    catch (ReturnException &returnValue)
    {
        result = returnValue.value;
        if (!result)
        {
            Error("Function return value is empty");
            result = std::make_shared<EmptyExpr>();
        }
        // if (result->getType() == ExprType::LITERAL)
        // {
        //    // LiteralExpr *literal = dynamic_cast<LiteralExpr *>(result.get());
        //     //if (literal->Value()->getType() != function->returnType)
        //    // {
        //     //      Error("Invalid function return type: "+ literal->Value()->toString());
        //    /// }
        // }
        // else
        // {
        //     Log(0, "Function return value: %s", result->toString().c_str());
        // }
    }
    this->environment = previous;
    this->currentDepth--;
    return result;
}



std::shared_ptr<Expr> Interpreter::visitNativeFunctionExpr(NativeFunctionExpr *expr)
{
    const std::string name = expr->name;
    int line = expr->line - 1;

    if (nativeFunctions.find(name) == nativeFunctions.end())
    {
        Error("Native function '" + name + "' at line: " + std::to_string(line) + " not defined");
        return std::make_shared<EmptyExpr>();
    }

    this->context.get()->values.clear();

    unsigned int numArgs = expr->parameters.size();
    for (const auto &arg : expr->parameters)
    {
        std::shared_ptr<Expr> value = evaluate(arg);
        if (!value)
        {
            Error("Invalid argument passed to function '" + name + "' at line: " + std::to_string(line));
            continue;
        }


        LiteralExpr *literal = dynamic_cast<LiteralExpr *>(value.get());
        if (!literal)
        {
            Error("Invalid argument passed to function '" + name + "' at line: " + std::to_string(line));
            return std::make_shared<EmptyExpr>();
        };
        Literal * exprValue = literal->Value();
       // Log(0, "Native function argument: %s", exprValue->toString().c_str());

       if (exprValue->getType() == LiteralType::INT)
          this->context.get()->add(Literal(exprValue->getInt()));
       else if (exprValue->getType() == LiteralType::FLOAT)
          this->context.get()->add(Literal(exprValue->getFloat()));
       else if (exprValue->getType() == LiteralType::BOOLEAN)
          this->context.get()->add(Literal(exprValue->getBool()));
       else if (exprValue->getType() == LiteralType::STRING)
          this->context.get()->add(Literal(exprValue->getString()));
       else
       {
           Error("Invalid argument passed to function '" + name + "' at line: " + std::to_string(line));
           return std::make_shared<EmptyExpr>();
       }
    }

     auto function = nativeFunctions[name];

     LiteralPtr result = function(this->context.get(), numArgs);

     if (!result)
    {
        Error("Invalid return value from native function '" + name + "' ." );
        return std::make_shared<EmptyExpr>();
    }
    if (result->getType() == LiteralType::INT)
        return std::make_shared<LiteralExpr>(result->getInt());
    else if (result->getType() == LiteralType::FLOAT)
        return std::make_shared<LiteralExpr>(result->getFloat());
    else if (result->getType() == LiteralType::BOOLEAN)
        return std::make_shared<LiteralExpr>(result->getBool());
    else if (result->getType() == LiteralType::STRING)
        return std::make_shared<LiteralExpr>(result->getString());
    else
    {
        Error("Invalid return value from native function '" + name + "' .");
        return std::make_shared<EmptyExpr>();
    }
}

std::shared_ptr<Expr> Interpreter::visitProcessCallExpr(ProcessCallExpr *expr)
{

    const std::string name = expr->name;
    int line = expr->line - 1;
    if (processList.find(name) == processList.end())
    {
        Error("Process '" + name + "' at line: " + std::to_string(line) + " not defined");
        return std::make_shared<EmptyExpr>();
    }
    unsigned long id = processID++;
    ProcessStmt *process = processList[name];
    if (!process)
    {
        Error("Process '" + name + "' die");
        return std::make_shared<EmptyExpr>();
    }

    unsigned int numArgsExpectd = process->parameter.size();
    unsigned int numArgs = expr->arguments.size();

    if (numArgs != numArgsExpectd)
    {
        Error("Incorrect number of arguments passed to process '" + name + "' at line: " + std::to_string(line) + " expected: " + std::to_string(numArgsExpectd) + " got: " + std::to_string(numArgs));
        return std::make_shared<EmptyExpr>();
    }
    this->currentDepth++;

    BlockStmt *block = dynamic_cast<BlockStmt *>(process->body.get());
    // const std::string &name, unsigned int ID, BlockStmt *block,const std::shared_ptr<Environment> &environment);
    std::unique_ptr<Process> newProcess = std::make_unique<Process>(this, name, id, block, this->environment.get());

    // std::shared_ptr<Environment> newEnv = std::make_shared<Environment>(this->currentDepth, this->environment);

    for (unsigned int i = 0; i < numArgs; i++)
    {
        std::shared_ptr<Expr> value = evaluate(expr->arguments[i]);
        if (!value)
        {
            Error("Invalid argument passed to process '" + name + "' at line: " + std::to_string(line));
            break;
        };
        std::string argName = process->parameter[i].get()->name;
        if (value->getType() == ExprType::LITERAL)
        {
            LiteralExpr *literal = dynamic_cast<LiteralExpr *>(value.get());

            if (!newProcess->define(argName, literal->value))
            {
                Error("Process variable '" + argName + "' already defined at line: " + std::to_string(line));
                return std::make_shared<EmptyExpr>();
            }
        }
        else
        {
            Error("Invalid argument passed to process '" + name + "" + value->toString());
            return std::make_shared<EmptyExpr>();
        }
    }

    // std::shared_ptr<BlockStmt> body = std::static_pointer_cast<BlockStmt>(process->body);

    // environmentStack.push(newEnv);
    // this->environment = newEnv;

    // std::shared_ptr<Expr> result = nullptr;
    // BlockStmt *block = dynamic_cast<BlockStmt *>(process->body.get());
    // try
    // {
    //     for (auto stmt : block->declarations)
    //     {
    //         execute(stmt);
    //     }
    // }
    // catch (ReturnException &returnValue)
    // {

    // }
    // environmentStack.pop();
    // if (!environmentStack.empty())
    // {
    //     this->environment = environmentStack.top();
    // }
    // else
    // {
    //     this->environment = globalEnvironment;
    // }
    processes.push_back(std::move(newProcess));
    this->currentDepth--;
    return Factory::Instance().createIntegerLiteral(id);
}

void Interpreter::visitFunctionStmt(FunctionStmt *stmt)
{
    if (functionList.find(stmt->name) != functionList.end())
    {
        Error("Function '" + stmt->name + "' already defined");
    }

    functionList[stmt->name] = stmt;
}

void Interpreter::visitProcessStmt(ProcessStmt *stmt)
{
    if (procedureList.find(stmt->name) != procedureList.end())
    {
        Error("Procedure '" + stmt->name + "' already defined");
        return;
    }
    processList[stmt->name] = stmt;
}

void Interpreter::visitReturnStmt(ReturnStmt *stmt)
{

    std::shared_ptr<Expr> value = evaluate(stmt->value);
    if (!value)
    {
        Error("RETURN: invalid  value ");
    }
    throw ReturnException(value);
}
void Interpreter::visitIfStmt(IfStmt *stmt)
{
    if (isTruthy(evaluate(stmt->condition)))
    {
        execute(stmt->thenBranch);
        return;
    }

    for (const auto &elif : stmt->elifBranch)
    {
        if (isTruthy(evaluate(elif->condition)))
        {
            execute(elif->thenBranch);
            return;
        }
    }

    if (stmt->elseBranch != nullptr)
    {
        execute(stmt->elseBranch);
    }
}

void Interpreter::visitWhileStmt(WhileStmt *stmt)
{
 if (!stmt)
    {
        Error("invalid while condition expression");
        return;
    }
    addressLoop = getAddress(stmt);
    while (true)
    {
        auto result = evaluate(stmt->condition);

        if (!result)
        {
            Error("invalid condition expression");
            addressLoop = 0x0;
            return;
        }

        if (!isTruthy(result))
        {
            break;
        }

        execute(stmt->body);
    }
    addressLoop = 0x0;
}

void Interpreter::visitBreakStmt(BreakStmt *stmt)
{
    // if (addressLoop == 0x0)
    // {
    //     Error("break outside of loop");
    //     return;
    // }
    throw BreakException();
}

void Interpreter::visitContinueStmt(ContinueStmt *stmt)
{
    // if (addressLoop == 0x0)
    // {
    //     Error("continue outside of loop");
    //     return;
    // }

    throw ContinueException();
}

void Interpreter::visitRepeatStmt(RepeatStmt *stmt)
{

    if (!stmt)
    {
        Error("invalid repeat condition expression");
        return;
    }
    try
    {
        addressLoop = getAddress(stmt);
        do
        {
            try
            {
                execute(stmt->body);
            }
            catch (ContinueException &)
            {
                // Do nothing, just continue the loop
            }

            auto result = evaluate(stmt->condition);
            if (!result)
            {
                Error("invalid condition expression");
                addressLoop = 0x0;
                return;
            }

            if (isTruthy(result))
            {
                break;
            }

        } while (true);
    }
    catch (BreakException &)
    {
        // Exit the loop
    }
    addressLoop = 0x0;
}

void Interpreter::visitLoopStmt(LoopStmt *stmt)
{

       if (!stmt)
        {
            Error("invalid repeat condition expression");
            return ;
        }
        try
        {
            addressLoop = getAddress(stmt);
            while(true)
            {
                try
                {
                    execute(stmt->body);
                }
                catch (ContinueException &)
                {
                    // Do nothing, just continue the loop
                }
            }
        }
        catch (BreakException &)
        {
            // Exit the loop
            return ;
        }
        addressLoop = 0x0;


}

void Interpreter::visitSwitchStmt(SwitchStmt *stmt)
{

    if (!stmt)
    {
        Error("invalid switch expression");
        return;
    }

    auto switch_value = evaluate(stmt->expression);
    if (!switch_value)
    {
        Error("invalid switch expression");
        return;
    }

    LiteralExpr *literal = dynamic_cast<LiteralExpr *>(switch_value.get());
    if (!literal)
    {
        Error("invalid switch expression");
        return;
    }

    for (const auto &caseStmt : stmt->cases)
    {

        auto result = evaluate(caseStmt->value);
        if (!result)
        {
            Error("invalid switch expression");
            return;
        }
        LiteralExpr *literalValue = dynamic_cast<LiteralExpr *>(result.get());
        if (Equal(literal, literalValue))
        {
            execute(caseStmt->body);
            return;
        }
    }
    if (stmt->default_case)
    {
        execute(stmt->default_case);
    }
}

void Interpreter::visitForStmt(ForStmt *stmt)
{
    if (!stmt)
    {
        Error("invalid for statement");
        return;
    }

    try
    {
        addressLoop = getAddress(stmt);
        if (stmt->initializer)
        {

            execute(stmt->initializer);
        }

        while (true)
        {
            if (stmt->condition)
            {
                auto conditionResult = evaluate(stmt->condition);
                if (!conditionResult)
                {
                    Error("invalid condition expression");
                    addressLoop = 0x0;
                    return;
                }

                if (!isTruthy(conditionResult))
                {

                    break;
                }
            }

            try
            {

                execute(stmt->body);
            }
            catch (ContinueException &)
            {
                // Continue to the next iteration of the loop
            }

            if (stmt->step)
            {

                evaluate(stmt->step);
            }
        }
    }
    catch (BreakException &)
    {
        // Exit the loop
    }
    addressLoop = 0x0;
}

bool Interpreter::isTruthy(const std::shared_ptr<Expr> &expr)
{
    if (!expr)
    {
        Warning("invalid condition expression");
        return false;
    }

    if (expr->getType() == ExprType::LITERAL)
    {
        LiteralExpr *literal = dynamic_cast<LiteralExpr *>(expr.get());
        if (literal && literal->Value())
        {
            return literal->value.isTruthy();
        }
    }

    Warning("non-literal expression in condition");
    return false;
}

//********************************************************************************** */

void Interpreter::Error(const Token &token, const std::string &message)
{
    panicMode = true;
    int line = token.line;
    std::string text = message + " at line: " + std::to_string(line);

    Log(2, text.c_str());
    throw FatalException(text);
}
void Interpreter::Error(const std::string &message)
{
    panicMode = true;

    std::string text = message;

    Log(2, text.c_str());
    throw FatalException(text);
}

bool Interpreter::Equal(LiteralExpr *a, LiteralExpr *b)
{

    if (!a || !b)
    {
        Error("invalid literals in Equal");
        return false;
    }
    return a->value.isEqual(b->value);
}

void Interpreter::Warning(const std::string &message)
{

    std::string text = message;
    Log(1, text.c_str());
}

void Interpreter::Info(const std::string &message)
{
    std::string text = "INFO: " + message;
    Log(0, text.c_str());
}


NativeFunction Interpreter::getNativeFunction(const std::string &name) const
{

    if (nativeFunctions.find(name) != nativeFunctions.end())
    {

        return nativeFunctions.at(name);
    }
    return nullptr;
}

bool Interpreter::isNativeFunctionDefined(const std::string &name) const
{
    return (nativeFunctions.find(name) != nativeFunctions.end());
}
void Interpreter::registerFunction(const std::string &name, NativeFunction function)
{
    if (functionList.find(name) != functionList.end())
    {
        Error("Function '" + name + "' already defined");
        return;
    }

    nativeFunctions[name] = function;
}
//*****************************************************************************************

Environment::Environment(int depth, Environment *parent) : m_depth(depth), m_parent(parent)
{
    // std::cout<<"Create Environment: "<< m_depth << std::endl;
}

Environment::~Environment()
{
     // std::cout<<"Delete Environment()"<< m_depth<<std::endl;
}

bool Environment::define(const std::string &name, const Literal &value)
{
   if (m_values.find(name) != m_values.end())
    {
        return false;
    }
    m_values[name] = value;
    return true;
}

Literal* Environment::get(const std::string &name)
{

    auto it = m_values.find(name);
    if (it != m_values.end())
    {
        return &(it->second);
    }
    if (m_parent != nullptr)
    {
        return m_parent->get(name);
    }
    return nullptr;

}

void Environment::set(const std::string &name, long value)
{
    Literal *literal = get(name);
    if (literal)
    {
        literal->setInt(value);
    }
}

void Environment::set(const std::string &name, double value)
{
    Literal *literal = get(name);
    if (literal)
    {
        literal->setFloat(value);
    }
}

void Environment::set(const std::string &name, unsigned char value)
{
    Literal *literal = get(name);
    if (literal)
    {
        literal->setByte(value);
    }
}

void Environment::set(const std::string &name, bool value)
{
    Literal *literal = get(name);
    if (literal)
    {
        literal->setBool(value);
    }
}

void Environment::set(const std::string &name, const std::string &value)
{
    Literal *literal = get(name);
    if (literal)
    {
        literal->setString(value);
    }
}

bool Environment::assign(const std::string &name, const Literal &value)
{
    auto it = m_values.find(name);
    if (it != m_values.end())
    {
        it->second.assign(value);
        return true;
    }
    if (m_parent != nullptr)
    {
        return m_parent->assign(name, value);
    }
    return false;
}

void Environment::remove(const std::string &name)
{

    m_values.erase(name);
    
}

void Environment::print()
{

     for (auto it = m_values.begin(); it != m_values.end(); ++it)
    {
        Log(0, "Variable: %s Value: %s", it->first.c_str(), it->second.toString().c_str());
    }
}

bool Environment::contains(const std::string &name)
{
    if (m_values.find(name) != m_values.end())
    {
        return true;
    }
    if (m_parent != nullptr)
    {
        if (m_parent->contains(name))
        {
            return true;
        }
    }

    return false;
}



bool Environment::addInteger(const std::string &name, long value)
{
    Literal literal;
    literal.setInt(value);
    m_values[name] = literal;

    return true;
}


bool Environment::addFloat(const std::string &name, double value)
{
    Literal literal;
    literal.setFloat(value);
    m_values[name] = literal;
    return true;

}

bool Environment::addByte(const std::string &name, unsigned char value)
{
    Literal literal;
    literal.setByte(value);
    m_values[name] = literal;

    return true;
}


bool Environment::addString(const std::string &name, const std::string &value)
{
    Literal literal;
    literal.setString(value);
    m_values[name] = literal;

    return true;
}

bool Environment::Put(const std::string &name, std::shared_ptr<Expr> value)
{
    if (m_expresions.find(name) != m_expresions.end())
    {
        Log(0, "Variable: %s Value: %s", name.c_str(), value->toString().c_str());
        m_expresions[name] = std::move(value);
        return true;
    }
    if (m_parent != nullptr)
    {
        Log(0, "Global Variable: %s Value: %s", name.c_str(), value->toString().c_str());
        m_parent->Put(name, std::move(value));

        return true;
    }

    return false;
}

std::shared_ptr<Expr> Environment::Remove(const std::string &name)
{

    if (m_expresions.find(name) != m_expresions.end())
    {
        std::shared_ptr<Expr> value = m_expresions[name];
        m_expresions.erase(name);
        return value;
    }
    if (m_parent != nullptr)
    {
        return m_parent->Remove(name);
    }
    return nullptr;    
}

bool Environment::addBool(const std::string &name, bool value)
{
    Literal literal;
    literal.setBool(value);
    m_values[name] = literal;

    return true;
}

// //*****************************************************************************************

Factory::Factory()
{
    
}

Factory::~Factory()
{

    clear();
}



std::shared_ptr<Literal> Factory::acquireInteger(long value)
{
    std::shared_ptr<Literal> literal = std::make_shared<Literal>( value);
    return literal;
}



std::shared_ptr<Literal> Factory::acquireFloat(double value)
{

       std::shared_ptr<Literal> literal = std::make_shared<Literal>( value);
    return literal;
}



std::shared_ptr<Literal> Factory::acquireByte(unsigned char value)
{

    std::shared_ptr<Literal> literal = std::make_shared<Literal>( value);
    return literal;
}

std::shared_ptr<Literal> Factory::acquireBool(bool value)
{
    std::shared_ptr<Literal> literal = std::make_shared<Literal>( value);
    return literal;
}

std::shared_ptr<Literal> Factory::acquireString(const std::string &value)
{

    std::shared_ptr<Literal> literal = std::make_shared<Literal>( value);
    return literal;
}







void Factory::clear()
{

 

    intPool.clear();
    floatPool.clear();
    bytePool.clear();
    boolPool.clear();
    stringPool.clear();


}



std::shared_ptr<LiteralExpr> Factory::createIntegerLiteral(long value)
{
      
        auto expression = std::make_shared<LiteralExpr>(value);
        return expression;
}

std::shared_ptr<LiteralExpr> Factory::createStringLiteral(const std::string &value)
{


   
        auto expression = std::make_shared<LiteralExpr>(value);
  
        return expression;
}

std::shared_ptr<LiteralExpr> Factory::createBoolLiteral(bool value)
{

      
        auto expression = std::make_shared<LiteralExpr>(value);
  
        return expression;
    
}

std::shared_ptr<LiteralExpr> Factory::createFloatLiteral(double value)
{


    
        auto expression = std::make_shared<LiteralExpr>( value);
        return expression;
  
}


std::shared_ptr<LiteralExpr> Factory::createByteLiteral(unsigned char value)
{

        auto expression = std::make_shared<LiteralExpr>( value);
        return expression;

}


LiteralPtr ExecutionContext::asInt(long value)
{
    auto literal = Factory::Instance().acquireInteger(value);


    return literal;
}



LiteralPtr ExecutionContext::asByte(unsigned char value)
{
    auto literal = Factory::Instance().acquireByte(value);

    return literal;
}
LiteralPtr ExecutionContext::asFloat(double value)
{

    auto literal = Factory::Instance().acquireFloat(value); 

    return literal; 
   
}
LiteralPtr ExecutionContext::asString(const char *value)
{

    auto literal = Factory::Instance().acquireString(value);

    return literal;
}

LiteralPtr ExecutionContext::asBool(bool value)
{

    auto literal = Factory::Instance().acquireBool(value);

    return literal;
    
}
LiteralPtr ExecutionContext::asString(const std::string &value)
{

    auto literal = Factory::Instance().acquireString(value);

    return literal;
}


void ExecutionContext::Error(const std::string &message)
{
    interpreter->Error(message);
}

void ExecutionContext::Info(const std::string &message)
{
    interpreter->Info(message);
}

Literal *ExecutionContext::Get(size_t index)
{

    if (index >= values.size())
    {
        interpreter->Error("Index out of range " + std::to_string(index));
        return nullptr;
    }
    return &values[index];
}

ExecutionContext::ExecutionContext(Interpreter *interpreter)
{
    this->interpreter = interpreter;

    values.reserve(25);
}

bool ExecutionContext::isTruthy(const LiteralPtr &value)
{
    return value->isTruthy();
}

bool ExecutionContext::isEqual(const LiteralPtr &lhs, const LiteralPtr &rhs)
{
    return lhs->isEqual(rhs.get());
}

long ExecutionContext::getInt(size_t index)
{
    Literal *literal = getLiteralInt(index);
    if (!literal)
    {
        return 0;
    }
    return literal->getInt();
    
}

double ExecutionContext::getFloat(size_t index)
{
    Literal *literal = getLiteralFloat(index);
    if (!literal)
    {
        return 0;
    }
    return literal->getFloat();
}

unsigned char ExecutionContext::getByte(size_t index)
{
    Literal *literal = getLiteralByte(index);
    if (!literal)
    {
        return 0;
    }
    return literal->getByte();
}

bool ExecutionContext::getBool(size_t index)
{
    Literal *literal = getLiteralBool(index);
    if (!literal)
    {
        return false;
    }
    return literal->getBool();
}


std::string ExecutionContext::getString(size_t index) 
{
    Literal *literal = getLiteralString(index);
    if (!literal)
    {
        return "";
    }
    return literal->getString();
}

Literal *ExecutionContext::getLiteralInt(size_t index)
{
  
   Literal *literal = Get(index);
   if (!literal)
   {
       interpreter->Error("Literal on index " + std::to_string(index) + " is null");
       return nullptr;
   }
  
   return literal;
}

Literal *ExecutionContext::getLiteralFloat(size_t index)
{

    Literal *literal = Get(index);
    if (!literal)
    {
        interpreter->Error("Literal on index " + std::to_string(index) + " is null");
        return nullptr;
    }
       return literal;
}

Literal *ExecutionContext::getLiteralByte(size_t index)
{
    Literal *literal = Get(index);
    if (!literal)
    {
        interpreter->Error("Literal on index " + std::to_string(index) + " is null");
        return nullptr;
    }
   return literal;
}

Literal *ExecutionContext::getLiteralString(size_t index)
{

    Literal *literal = Get(index);
    if (!literal)
    {
        interpreter->Error("Literal on index " + std::to_string(index) + " is null");
        return nullptr;
    }
    return literal;
}

Literal *ExecutionContext::getLiteralBool(size_t index)
{
    Literal *literal = Get(index);
    if (!literal)
    {
        interpreter->Error("Literal on index " + std::to_string(index) + " is null");
        return nullptr;
    }
   return literal;
}

void ExecutionContext::add(const Literal &value)
{
    values.push_back(value);
}
