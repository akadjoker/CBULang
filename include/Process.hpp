#pragma once
#include "Token.hpp"
#include "Literal.hpp"

class Environment;
class BlockStmt;
class Interpreter;
class Stmt;
class LoopStmt;

class Process

{

    std::string name;
    unsigned int ID;
    bool m_running;
    std::shared_ptr<Environment> environment;
    BlockStmt *block;
    Interpreter *interpreter;
    std::vector<std::shared_ptr<Stmt>> initialStatements;
    std::vector<BlockStmt*> loopStatements;
    std::vector<std::shared_ptr<Stmt>> finalStatements;
    int state;
    friend class Interpreter;

public:
    Process(Interpreter *i,const std::string &name, unsigned int ID, BlockStmt *block,const std::shared_ptr<Environment> &environment);

    virtual ~Process();

    void run();

    void render();

    bool running() const    {        return m_running;    }

    const std::string &getName() const    {        return name;    }
    bool define(const std::string &name, const std::shared_ptr<Literal> &value);

    void load(BlockStmt *block);

    //LOCALS
    double X;
    double Y;
};
