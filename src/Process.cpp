#include "pch.h"
#include "Process.hpp"
#include "Interpreter.hpp"
#include "Literal.hpp"
#include "Utils.hpp"
#include "Stm.hpp"
#include "Math.hpp"

#include <raylib.h>


Process::Process(Interpreter *i,const std::string &name, long ID,size_t index)
{

    this->interpreter = i;
    this->index = index;
    this->name = name;
    this->ID = ID;
    this->m_running = true;
    this->graph = 0;
    this->layer = 0;
    this->instance = Scene::Get().CreateInstance(ID, name, graph, 0, 0, 0, layer);
    _lastGraph = -1;
    _lastLayer = -1;
    parent = nullptr;

    if (interpreter->getContext())
    {
        parent = interpreter->getContext()->getInternalProcess();
        //if (parent)
        //    Log(0, "Create Process(%s) id(%d) parent  id(%d) name(%s)", name.c_str(), ID, parent->ID, parent->name.c_str());
    }

    

    state = 0;



  


    
}




Process::~Process()
{
    instance = nullptr;
    //  std::cout<<"Delete Process("<<ID<<")"<<std::endl;
}

void Process::run()
{
    if (!m_running)  return;


    interpreter->enterLocal(environment);
 
     //

     
     //environment->print();

 

     try
     {
         switch (state)
         {
         case 0:
         {
                    this->interpreter->processInitExecute(index);
                    // for (auto &stmt : initialStatements)
                    // {
                    //     interpreter->execute(stmt);
                    // }
                    pre_run();
             state = 1;
             break;
         }
         case 1:
         {
             try
             {

                 try
                 {
                     pre_run();
                     interpreter->processLoopExecute(index);
                     post_run();
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

             
         }
         break;
         case 2:
         {
            pre_run();
            interpreter->processEndExecute(index);
            //  for (auto &stmt : finalStatements)
            //  {
            //      interpreter->execute(stmt);
            //  }
             post_run();
             state = 3;
             break;
         }
         case 3:
         {
            if (this->instance!=nullptr)
               this->instance->Destroy();
             m_running = false;
             break;
         }
         }
       
     }
     catch (ReturnException &returnValue)
     {
        state = 3;
     }

     pre_run();
     post_run();

     interpreter->exitBlock();

     //  environment->print();
     




}

void Process::pre_run()
{

    this->instance->x = environment->get("x")->getFloat();
    this->instance->y = environment->get("y")->getFloat();
    this->instance->angle = environment->get("angle")->getFloat();

    this->instance->scale.x = environment->get("scale_x")->getFloat();
    this->instance->scale.y = environment->get("scale_y")->getFloat();
    
    this->instance->skew.x = environment->get("skew_x")->getFloat();
    this->instance->skew.y = environment->get("skew_y")->getFloat();
    
    this->instance->color.r = environment->get("red")->getByte();
    this->instance->color.g = environment->get("green")->getByte();
    this->instance->color.b = environment->get("blue")->getByte();
    this->instance->color.a = environment->get("alpha")->getByte();

    this->instance->showBox = environment->get("show_box")->getBool();

    this->instance->visible = environment->get("visible")->getBool();
    this->instance->active = environment->get("active")->getBool();
    


    layer = environment->get("layer")->getInt();
    graph =  environment->get("graph")->getInt();
    if (_lastGraph != graph)
    {
        _lastGraph = graph;
        this->instance->setGraph(graph);
    }
    if (_lastLayer !=layer) 
    {
        _lastLayer = layer;
        this->instance->layer = layer;
    }
}

void Process::post_run()
{
    environment->get("x")->setFloat(instance->x);
    environment->get("y")->setFloat(instance->y);
    environment->get("angle")->setFloat(instance->angle);
    
    // environment->get("scale_x")->setFloat(instance->scale.x);
    // environment->get("scale_y")->setFloat(instance->scale.y);
    
    // environment->get("skew_x")->setFloat(instance->skew.x);
    // environment->get("skew_y")->setFloat(instance->skew.y);


    // environment->get("red")->setByte(instance->color.r);
    // environment->get("green")->setByte(instance->color.g);
    // environment->get("blue")->setByte(instance->color.b);
    // environment->get("alpha")->setByte(instance->color.a);

    // environment->get("show_box")->setBool(instance->showBox);

    // environment->get("visible")->setBool(instance->visible);
    // environment->get("active")->setBool(instance->active);

    

}

void Process::render()
{
    

   // environment->print();


  #if defined(USE_GRAPHICS)
    //DrawCircle(X, Y, 10, RED);
    #endif

}

bool Process::define(const std::string &name, const Literal &value)
{
    return this->environment->define(name, value);
}

void Process::advance(double speed)
{   
    this->instance->x += speed *  cos_deg(this->instance->angle);
    this->instance->y += speed * -sin_deg(this->instance->angle);

}

void Process::xadvance(double speed, double _angle)
{
    this->instance->x += speed * cos_deg(_angle);
    this->instance->y += speed * -sin_deg(_angle);
}

void Process::rotate_to(double target_angle, double t) 
{
    this->instance->angle = lerp_angle(this->instance->angle, target_angle, t);
}