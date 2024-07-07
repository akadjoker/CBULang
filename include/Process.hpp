#pragma once
#include "Token.hpp"
#include "Literal.hpp"

class Process
{

    std::string name;
    unsigned int ID;
    bool running;

public:
    Process(const std::string &name, unsigned int ID) :
    name(name), ID(0)
    {
        this->running = true;
    }

    virtual ~Process(){}

    const std::string &getName() const    {        return name;    }

  
};



