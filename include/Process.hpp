#pragma once
#include "Token.hpp"
#include "Literal.hpp"

#if defined(USE_GRAPHICS) 
#include "Core.hpp" 
#endif

class Environment;
class BlockStmt;
class Interpreter;
class Stmt;
class LoopStmt;


struct ProcessExecution
{
    long index;
    std::string name;
    std::vector<std::shared_ptr<Stmt>> initialStatements;
    std::vector<BlockStmt*>            loopStatements;
    std::vector<std::shared_ptr<Stmt>> finalStatements;
    ProcessExecution() = default;
    ~ProcessExecution()
    {
        initialStatements.clear();
        loopStatements.clear();
        finalStatements.clear();
    }
};

class Process

{

    std::string name;
    bool m_running;
    std::shared_ptr<Environment> environment;
    Interpreter *interpreter;
    
    size_t index;
    int state;
    friend class Interpreter;

public:
    Process(Interpreter *i,const std::string &name, long ID,size_t index);

    virtual ~Process();

    void run();
    void pre_run();
    void post_run();


    void render();

    bool running() const    {        return m_running;    }

    const std::string &getName() const    {        return name;    }
    bool define(const std::string &name, const Literal &value);

    void load(BlockStmt *block);

    void advance(double speed);
    void xadvance(double speed,double angle);
    void rotate_to(double target_angle, double t);

   
    long graph;
    long layer;
    Instance* instance;
    long ID;
    long _lastGraph;
    long _lastLayer;
    Process *parent;
};
