#include "pch.h"
#include "Process.hpp"
#include "Interpreter.hpp"
#include "Literal.hpp"
#include "Utils.hpp"
#include "Stm.hpp"

#define USE_RAYLIB
//#undef USE_RAYLIB

#ifdef USE_RAYLIB
#include <raylib.h>
#endif

Process::Process(Interpreter *i,const std::string &name, long ID, BlockStmt *block)
{

    this->interpreter = i;
    this->block = block;
    this->name = name;
    this->ID = ID;
    this->m_running = true;
    this->graph = 0;


      //std::cout<<"Create Process("<<ID<<")"<<std::endl;

    state = 0;
    bool isEnds = false;

    for (auto &stmt : block->declarations)
    {
            if (stmt->getType() == StmtType::LOOP)
            {
                    LoopStmt *loop  = dynamic_cast<LoopStmt *>(stmt.get());
                    BlockStmt *child = dynamic_cast<BlockStmt *>(loop->body.get());
                    loopStatements.push_back(child);

                    isEnds = true;
                    continue;
            }
    
        
        if (isEnds)
        {
              finalStatements.push_back(stmt);
        } else 
        {
              initialStatements.push_back(stmt);
        }
    }



  


    
}




Process::~Process()
{
  //  std::cout<<"Delete Process("<<ID<<")"<<std::endl;
}

void Process::run()
{
    if (!m_running)  return;



 
     //

     interpreter->enterLocal(environment);

 

     try
     {
         switch (state)
         {
         case 0:
         {
             for (auto &stmt : initialStatements)
             {
                 interpreter->execute(stmt);
             }

             state = 1;
             break;
         }
         case 1:
         {
             try
             {

                 try
                 {
                     for (auto &stmt : loopStatements)
                     {
                         // if (stmt->getType() == StmtType::BLOCK)
                         // {
                         //     interpreter->executeBlock(stmt, environment);
                         //     continue;
                         // }
                         // interpreter->executeBlock(stmt, environment);
                         interpreter->execute(stmt);
                     }
                 }
                 catch (ContinueException &)
                 {
                     // Do nothing, just continue the loop
                 }
             }
             catch (BreakException &)
             {

                 state = 2;
             }

             render();
         }
         break;
         case 2:
         {
             for (auto &stmt : finalStatements)
             {
                 interpreter->execute(stmt);
             }
             state = 3;
             break;
         }
         case 3:
             m_running = false;
             break;
         }
       
     }
     catch (ReturnException &returnValue)
     {
        state = 3;
     }
    

    interpreter->exitBlock();


     X = environment->get("x")->getFloat();
     Y = environment->get("y")->getFloat();

}

void Process::render()
{
    

   // environment->print();

   #ifdef USE_RAYLIB

    DrawCircle(X, Y, 10, RED);

   #endif

}

bool Process::define(const std::string &name, const Literal &value)
{
    return this->environment->define(name, value);
}

