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
    globalEnvironment = std::make_shared<Environment>(0, nullptr);
    environmentStack.push(globalEnvironment);
    environment = globalEnvironment;
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

float Interpreter::time_elapsed()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now - start_time;
    auto milliseconds = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(duration).count();
    return milliseconds;

    // auto now = std::chrono::system_clock::now();
    // auto duration = now - start_time;//now.time_since_epoch();
    // auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    // return seconds;
}

std::shared_ptr<Expr> Interpreter::visit(const std::shared_ptr<Expr> &expr)
{
    return expr->accept(this);
}

static bool literalToBool(const Literal &literal)
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

#include "InterpreterExp.cc"

std::shared_ptr<Expr> Interpreter::visitNowExpr(NowExpr *expr)
{
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::duration<double>>(duration).count();

    return createFloatLiteral(static_cast<double>(seconds));
}

std::shared_ptr<Expr> Interpreter::visitEmptyExpr(EmptyExpr *expr)
{
    return std::make_shared<EmptyExpr>();
}

std::shared_ptr<Expr> Interpreter::evaluate(const std::shared_ptr<Expr> &expr)
{
    if (!expr)
    {
        Warning("Evaluate null expression");
        return std::make_shared<EmptyExpr>();
    }
    // std::cout<<"evaluate: "<<expr->toString()<<std::endl;
    return expr->accept(this);
}

void Interpreter::execute(Stmt *statement)
{
    statement->accept(this);
}

void Interpreter::build(std::shared_ptr<Stmt> program)
{
       execute(program);

    // Program *p = dynamic_cast<Program *>(program.get());

    // auto task = [this, p]() mutable
    // {
    //     auto env = std::make_shared<Environment>(this->currentDepth, nullptr);
    //     this->environment = env;
    //     visitProgram(p);

    //    // auto coroutine = coroutines.front();
    //   ///  coroutine->finish();
    // };
    // addCoroutine(std::make_shared<Coroutine>(task));
}

// void Interpreter::execute(const std::shared_ptr<Stmt> &statement)
// {
//    std::cout<<"execute: "<<statement->toString()<<std::endl;
//     statement->accept(this);
// }

void Interpreter::execute(const std::shared_ptr<Stmt> &stmt)
{
    if (stmt == nullptr)
        return;
   // std::cout << "execute: " << stmt->toString() << std::endl;
    stmt->accept(this);

}


void Interpreter::executeBlock(BlockStmt *stmt, const std::shared_ptr<Environment> &env)
{
    environmentStack.push(environment);
    environment = env;

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

    environment = environmentStack.top();
    environmentStack.pop();
}

bool Interpreter::run()
{


    for (size_t i = 0; i < processes.size(); i++)
    {
      
        environmentStack.push(environment);
        environment = processes[i]->environment;

        if (!processes[i]->running())
        {
            remove_processes.push_back(std::move(processes[i]));
            processes.erase(processes.begin() + i);
            i--;
            continue;
        }
        processes[i]->run();
        

        environment = environmentStack.top();
        environmentStack.pop();
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

        if (literal->value->getType() == LiteralType::INT)
        {
            Log(3, "%d", literal->value->getInt());
        }
        else if (literal->value->getType() == LiteralType::FLOAT)
        {
            Log(3, "%f", literal->value->getFloat());
        }
        else if (literal->value->getType() == LiteralType::BOOLEAN)
        {
            Log(3, "%s", literal->value->getBool() ? "true" : "false");
        }
        else if (literal->value->getType() == LiteralType::STRING)
        {
            if (literal->value->getString() == nullptr)
            {
                Log(3, "NULL");
            }
            else
            {
                Log(3, "%s", literal->value->getString());
            }
        }
        else if (literal->value->getType() == LiteralType::POINTER)
        {
            Log(3, "%p", literal->value->getPointer());
        }
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

    Literal *value = this->environment->get(name).get();
    if (!value)
    {
        Error("Load variable  '" + name + "' is null at line: " + std::to_string(line));
        return std::make_shared<EmptyExpr>();
    }

    //   return std::make_shared<LiteralExpr>(std::make_shared<Literal>(value));

    if (value->getType() == LiteralType::INT)
    {
        return createIntLiteral(value->getInt());
    }
    else if (value->getType() == LiteralType::FLOAT)
    {
        return createFloatLiteral(value->getFloat());
    }
    else if (value->getType() == LiteralType::BOOLEAN)
    {
        return createBoolLiteral(value->getBool());
    }
    else if (value->getType() == LiteralType::STRING)
    {
        return createStringLiteral(value->getString());
    }
    else if (value->getType() == LiteralType::POINTER)
    {
        return createPointerLiteral(value->getPointer());
    }

    return std::make_shared<EmptyExpr>();
}

std::shared_ptr<Expr> Interpreter::visitAssignExpr(AssignExpr *expr)
{
    std::string name = expr->name.lexeme;

    auto value = evaluate(expr->value);
    Literal *oldLiteral = this->environment->get(name).get();

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

        if (oldLiteral->getType() == LiteralType::INT && literal->value->getType() == LiteralType::FLOAT)
        {
            this->environment->assign(name, literal->value);
        }
        else if (oldLiteral->getType() == LiteralType::FLOAT && literal->value->getType() == LiteralType::INT)
        {
            this->environment->assign(name, literal->value);
        }
        else if (oldLiteral->getType() == literal->value->getType())
        {
            this->environment->assign(name, literal->value);
        }
        else
        {
            Error(expr->name, "Variable  '" + name + "' type not match");
            return std::make_shared<EmptyExpr>();
        }

        this->environment->assign(name, literal->value);
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

void Interpreter::visitBlockStmt(BlockStmt *stmt)
{
    executeBlock(stmt, environment);
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
    std::shared_ptr<Environment> newEnv = std::make_shared<Environment>(this->currentDepth, this->environment);

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
    std::shared_ptr<Environment> newEnv = std::make_shared<Environment>(this->currentDepth, this->environment);
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
            result = std::make_shared<EmptyExpr>();
        if (result->getType() == ExprType::LITERAL)
        {
            LiteralExpr *literal = dynamic_cast<LiteralExpr *>(result.get());
            if (literal->value->getType() != function->returnType)
            {
                this->environment = previous;
                Error("Invalid function return type");
            }
        }
        else
        {
            Log(0, "Function return value: %s", result->toString().c_str());
        }
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

    unsigned int numArgs = expr->parameters.size();
    int index = 0;
    for (const auto &arg : expr->parameters)
    {
        std::shared_ptr<Expr> value = evaluate(arg);
        if (!value)
        {
            Error("Invalid argument passed to function '" + name + "' at line: " + std::to_string(line));
            continue;
        }

        if (value->getType() != ExprType::LITERAL)
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
        native_args[index++] = literal->value;
    }

    auto function = nativeFunctions[name];
    LiteralPtr result = function(this->context.get(), numArgs, native_args);

    return std::make_shared<LiteralExpr>(result);
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
    std::unique_ptr<Process> newProcess = std::make_unique<Process>(this, name, id, block, this->environment);

    // std::shared_ptr<Environment> newEnv = std::make_shared<Environment>(this->currentDepth, this->environment);

    for (unsigned int i = 0; i < numArgs; i++)
    {
        std::shared_ptr<Expr> value = evaluate(expr->arguments[i]);
        if (!value)
        {
            Error("Invalid argument passed to process '" + name + "' at line: " + std::to_string(line));
            return std::make_shared<EmptyExpr>();
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
    return createIntLiteral(id);
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

    // scheduler.addTask(std::make_unique<WhileTask>(stmt, this));

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
        if (literal && literal->value)
        {
            return literal->value->isTrue();
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
        Error("invalid literal match");
        return false;
    }
    if (a->getType() != b->getType())
    {
        Error("invalid literal match type");
        return false;
    }

    if (a->value->getType() == LiteralType::BOOLEAN && b->value->getType() == LiteralType::BOOLEAN)
    {
        return (a->value->getBool() == b->value->getBool());
    }
    else if (a->value->getType() == LiteralType::STRING && b->value->getType() == LiteralType::STRING)
    {
        return (a->value->getString() == b->value->getString());
    }
    else if (a->value->getType() == LiteralType::INT && b->value->getType() == LiteralType::INT)
    {
        return (a->value->getInt() == b->value->getInt());
    }
    else if (a->value->getType() == LiteralType::FLOAT && b->value->getType() == LiteralType::FLOAT)
    {
        return (a->value->getFloat() == b->value->getFloat());
    }
    return false;
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

std::shared_ptr<LiteralExpr> Interpreter::createIntLiteral(int value)
{

    return LiteralPool::createIntLiteral(value);
}

std::shared_ptr<LiteralExpr> Interpreter::createFloatLiteral(double value)
{
    return LiteralPool::createFloatLiteral(value);
}

std::shared_ptr<LiteralExpr> Interpreter::createStringLiteral(const std::string &value)
{
    return LiteralPool::createStringLiteral(value);
}

std::shared_ptr<LiteralExpr> Interpreter::createBoolLiteral(bool value)
{

    return LiteralPool::createBoolLiteral(value);
}

std::shared_ptr<LiteralExpr> Interpreter::createPointerLiteral(void *value)
{
    return LiteralPool::createPointerLiteral(value);
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

Environment::Environment(int depth, const std::shared_ptr<Environment> &parent) : m_depth(depth), m_parent(parent)
{
    // std::cout<<"Create Environment: "<< m_depth << std::endl;
}

Environment::~Environment()
{
    //  std::cout<<"Delete Environment()"<< m_depth<<std::endl;
}

bool Environment::define(const std::string &name, const std::shared_ptr<Literal> &value)
{
    if (m_values.find(name) != m_values.end())
    {
        return false;
    }
    m_values[name] = value;
    return true;
}

std::shared_ptr<Literal> Environment::get(const std::string &name)
{

    if (m_values.find(name) != m_values.end())
    {
        return m_values[name];
    }
    if (m_parent != nullptr)
    {
        if (m_parent->contains(name))
        {
            return m_parent->get(name);
        }
    }

    return nullptr;
}

bool Environment::assign(const std::string &name, const std::shared_ptr<Literal> &value)
{
    if (contains(name))
    {
        Literal *literal = m_values[name].get();
        if (literal != nullptr)
            literal->copyFrom(value.get());
    }

    if (m_parent != nullptr)
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
    if (m_parent != nullptr)
    {
        if (m_parent->contains(name))
        {
            return true;
        }
    }

    return false;
}

bool Environment::addInt(const std::string &name, int value)
{
    auto literal = LiteralPool::Instance().acquireInt(value);
    return define(name, literal);
}

bool Environment::addFloat(const std::string &name, double value)
{
    auto literal = LiteralPool::Instance().acquireFloat(value);
    return define(name, literal);
}

bool Environment::addString(const std::string &name, const std::string &value)
{
    auto literal = LiteralPool::Instance().acquireString(value);
    return define(name, literal);
}

bool Environment::addBool(const std::string &name, bool value)
{
    auto literal = LiteralPool::Instance().acquireBool(value);
    return define(name, literal);
}

// //*****************************************************************************************

LiteralPool::LiteralPool()
{
}

LiteralPool::~LiteralPool()
{

    clear();
}

void LiteralPool::releaseLiteral(std::unique_ptr<Literal> literal)
{
    if (pool.size() < maxPoolSize)
    {
        literal->clear();
        pool.push_back(std::move(literal));
    }
}

std::shared_ptr<Literal> LiteralPool::acquireLiteral()
{
    if (pool.empty())
    {
        return std::make_shared<Literal>();
    }
    std::shared_ptr<Literal> literal(pool.back().release(),
                                     [this](Literal *p)
                                     {
                                         this->releaseLiteral(std::unique_ptr<Literal>(p));
                                     });
    pool.pop_back();
    return literal;
}

void LiteralPool::init(size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        pool.push_back(std::make_unique<Literal>());
    }
}

std::shared_ptr<Literal> LiteralPool::acquireInt(int value)
{

    std::shared_ptr<Literal> literal = acquireLiteral();
    literal->setInt(value);
    return literal;
}

std::shared_ptr<Literal> LiteralPool::acquireFloat(double value)
{

    std::shared_ptr<Literal> literal = acquireLiteral();
    literal->setFloat(value);
    return literal;
}

std::shared_ptr<Literal> LiteralPool::acquireBool(bool value)
{

    std::shared_ptr<Literal> literal = acquireLiteral();
    literal->setBool(value);
    return literal;
}

std::shared_ptr<Literal> LiteralPool::acquireString(const std::string &value)
{

    std::shared_ptr<Literal> literal = acquireLiteral();
    literal->setString(value.c_str());
    return literal;
}

std::shared_ptr<Literal> LiteralPool::acquirePointer(void *value)
{

    std::shared_ptr<Literal> literal = acquireLiteral();
    literal->setPointer(value);
    return literal;
}

void LiteralPool::release(std::shared_ptr<Literal> literal)
{
    // literal->clear();
    // pool.push_back(std::move((std::unique_ptr<Literal>(literal.get()))));
    // literal.release();
}

void LiteralPool::clear()
{

    pool.clear();
}

std::shared_ptr<LiteralExpr> LiteralPool::createIntLiteral(int value)
{
    if (USE_POOL)
        return std::make_shared<LiteralExpr>(LiteralPool::Instance().acquireInt(value));
    else
        return std::make_shared<LiteralExpr>(std::move(std::make_shared<Literal>(value)));
}

std::shared_ptr<LiteralExpr> LiteralPool::createFloatLiteral(double value)
{
    if (USE_POOL)
        return std::make_shared<LiteralExpr>(LiteralPool::Instance().acquireFloat(value));
    else
        return std::make_shared<LiteralExpr>(std::move(std::make_shared<Literal>(value)));
}

std::shared_ptr<LiteralExpr> LiteralPool::createStringLiteral(const std::string &value)
{
    // if (USE_POOL)
    //    return std::make_shared<LiteralExpr>(LiteralPool::Instance().acquireString(value));
    // else
    return std::make_shared<LiteralExpr>(std::move(std::make_shared<Literal>(value.c_str())));
}

std::shared_ptr<LiteralExpr> LiteralPool::createBoolLiteral(bool value)
{
    if (USE_POOL)
        return std::make_shared<LiteralExpr>(LiteralPool::Instance().acquireBool(value));
    else
        return std::make_shared<LiteralExpr>(std::move(std::make_shared<Literal>(value)));
}

std::shared_ptr<LiteralExpr> LiteralPool::createPointerLiteral(void *value)
{
    if (USE_POOL)
        return std::make_shared<LiteralExpr>(LiteralPool::Instance().acquirePointer(value));
    else
        return std::make_shared<LiteralExpr>(std::move(std::make_shared<Literal>(value)));
}

LiteralPtr ExecutionContext::asInt(int value)
{
    return std::make_shared<Literal>(value);
}

LiteralPtr ExecutionContext::asFloat(double value)
{
    return std::make_shared<Literal>(value);
}

LiteralPtr ExecutionContext::asString(const char &value)
{
    return std::make_shared<Literal>(value);
}

LiteralPtr ExecutionContext::asBool(bool value)
{
    return std::make_shared<Literal>(value);
}

void ExecutionContext::Error(const std::string &message)
{
    interpreter->Error(message);
}

void ExecutionContext::Info(const std::string &message)
{
    interpreter->Info(message);
}

ExecutionContext::ExecutionContext(Interpreter *interpreter)
{
    this->interpreter = interpreter;
}

bool ExecutionContext::isTruthy(const LiteralPtr &value)
{
    return value->isTrue();
}

bool ExecutionContext::isEqual(const LiteralPtr &lhs, const LiteralPtr &rhs)
{
    return lhs->isEqual(rhs.get());
}

bool ExecutionContext::isString(const LiteralList &value)
{

    return value->getType() == LiteralType::STRING;
}

bool ExecutionContext::isInt(const LiteralList &value)
{
    return value->getType() == LiteralType::INT;
}

bool ExecutionContext::isFloat(const LiteralList &value)
{
    return value->getType() == LiteralType::FLOAT;
}

bool ExecutionContext::isBool(const LiteralList &value)
{
    return value->getType() == LiteralType::BOOLEAN;
}

LiteralPtr ExecutionContext::asString(const std::string &value)
{
    return std::make_shared<Literal>(value.c_str());
}
