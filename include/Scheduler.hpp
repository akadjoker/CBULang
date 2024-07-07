#pragma once

#include <queue>



class Interpreter;
class WhileStmt;
class LoopStmt;
class Program;
class SwitchStmt;

class Task
{
public:
    virtual ~Task() = default;
    virtual bool run() = 0; // Retorna true se a tarefa foi concluída
};

class Scheduler
{
private:
    std::queue<std::unique_ptr<Task>> tasks;

public:
    void addTask( std::unique_ptr<Task> task);

    void run();

    void clear();

    bool empty();

    size_t size();

};

class ProgramTask : public Task 
{
private:
    std::shared_ptr<Program> program; 
    Interpreter *interpreter;

public:
    ProgramTask(std::shared_ptr<Program> p, Interpreter *i) : program(std::move(p)), interpreter(i) {}

    bool run() override;

};

class WhileTask : public Task
{
private:
    WhileStmt *stmt;
    Interpreter *interpreter;

public:
    WhileTask(WhileStmt *s, Interpreter *i) : stmt(s), interpreter(i) {}

    bool run() override;
};

class LoopTask : public Task
{
private:
    LoopStmt *stmt;
    Interpreter *interpreter;

public:
    LoopTask(LoopStmt *s, Interpreter *i) : stmt(s), interpreter(i) {}

    bool run() override;
};
