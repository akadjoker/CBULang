#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Token.hpp"


    enum LiteralType
    {
        STRING,
        INT,
        FLOAT,
        BOOLEAN,
        POINTER
    };

class Literal
{
private:
    union
    {
        char *stringVal;
        int   intVal;
        float floatVal;
        bool  boolVal;
        void *pointerVal;

    };
    
    LiteralType type;

    std::string ltString(LiteralType type) const
    {
        switch (type)
        {
            case STRING:
                return "String";
            case INT:
                return "Int";
            case FLOAT:
                return "Float";
            case BOOLEAN:
                return "Boolean";
            case POINTER:
                return "Pointer";
        }
        return "";
    }

public:
    
    Literal(int value)
    {
        type = INT;
        intVal = value;
    }
    Literal(float value)
    {
        type = FLOAT;
        floatVal = value;
    }
    Literal(const std::string &value)
    {
        type = STRING;
        stringVal = strdup(value.c_str());
    }

    Literal(bool value)
    {
        type = BOOLEAN;
        boolVal = value;
    }

    Literal(void *value)
    {
        type = POINTER;
        pointerVal = value;
    }

    virtual ~Literal() 
    {
        if (type == STRING) 
        {
            free(stringVal);
            stringVal = nullptr;
        }
        if (type == POINTER)
        {
            free(pointerVal);
            pointerVal = nullptr;
        }
    }
    
    
    std::string toString() const
    {
        switch (type)
        {
            case STRING:
                return "String: " + std::string(stringVal);
            case INT:
                return "Int: " + std::to_string(intVal);
            case FLOAT:
                return "Float: " + std::to_string(floatVal);
            case BOOLEAN:
            {
                if (boolVal)
                    return "Boolean: true";
                else
                    return "Boolean: false";
                
            }
            case POINTER:
            {
                if (pointerVal == nullptr)
                    return "Pointer: nil";
                 else
                 {
                    int ref = *(int*)pointerVal;
                    return "Pointer" + std::to_string(ref);
                 }
            }
        }
        return "Unknown";
    }

    LiteralType getType() const
    {
        return type;
    } 


    char* getString() const
    {
        if (type != STRING)
        {
            std::cout<<"Invalid string literal type"<<std::endl;
            return nullptr;
        }
        return stringVal;
    }

    int getInt() const
    {
        if (type != INT)
        {
            std::cout<<"Invalid int literal type"<<std::endl;
            return INT32_MIN;
        }
        return intVal;
    }

    float getFloat() const
    {
        if (type != FLOAT)
        {
            std::cout<<"Invalid int literal type"<<std::endl;
            return -MAXFLOAT;
        }
        return floatVal;
    }

    bool getBool() const
    {
        if (type != BOOLEAN)
        {
            std::cout<<"Invalid int literal type"<<std::endl;
            return false;
        }
        return boolVal;
    }

    void* getPointer() const
    {
        if (type != POINTER)
        {
            std::cout<<"Invalid int literal type"<<std::endl;
            return nullptr;
        }
        return pointerVal;
    }

    void setPointer(void *value)
    {
        if (type != POINTER)
        {
            std::cout<<"Invalid pointer literal type"<<std::endl;
            return;
        }
        pointerVal = value;
    }

    void setString(const std::string &value)
    {
        if (type != STRING)
        {
            std::cout<<"Invalid string literal type"<<std::endl;
            return;
        }
        free(stringVal);
        stringVal = strdup(value.c_str());
    }

    void setInt(int value)
    {
        if (type != INT)
        {
            std::cout<<"Invalid int literal type"<<std::endl;
            return;
        }
        intVal = value;
    }

    void setFloat(float value)
    {
        if (type != FLOAT)
        {
            std::cout<<"Invalid float literal type"<<std::endl;
            return;
        }
        floatVal = value;
    }
    void setBool(bool value)
    {
        if (type != BOOLEAN)
        {
            std::cout<<"Invalid bool literal type"<<std::endl;
            return;
        }
        boolVal = value;
    }
    

   

   

 
    
    
};