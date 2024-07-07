#include "pch.h"
#include "Scheduler.hpp"
#include "Interpreter.hpp"
#include "Exp.hpp"
#include "Stm.hpp"


void Scheduler::addTask(std::unique_ptr<Task> task)
{
    
    tasks.push(std::move(task));
}

void Scheduler::run()
{
    
        while (!tasks.empty())
        {
            auto &task = tasks.front();
            bool completed = task->run();
            if (completed)
            {
                tasks.pop();
            }
            else
            {

                tasks.push(std::move(tasks.front()));
                tasks.pop();
            }
        }
    
}

void Scheduler::clear()
{

   while (!tasks.empty())
   {
       tasks.pop();
   }
}

bool Scheduler::empty()
{

    return tasks.empty();
}

size_t Scheduler::size()
{

    return tasks.size();
}

bool WhileTask::run()
{
   
    return true; 

}

bool LoopTask::run()
{
    return interpreter->executeLoop(stmt);
}

bool ProgramTask::run()
{
    interpreter->execute(program.get()->statement);
    return true;
}
