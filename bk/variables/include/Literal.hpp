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


    char* getStringValue() const
    {
        if (type != STRING)
        {
            throw std::runtime_error("Invalid string literal type");
        }
        return stringVal;
    }

    int getIntValue() const
    {
        if (type != INT)
        {
            throw std::runtime_error("Invalid int literal type");
        }
        return intVal;
    }

    float getFloatValue() const
    {
        if (type != FLOAT)
        {
            throw std::runtime_error("Invalid float literal type");
        }
        return floatVal;
    }

    bool getBoolValue() const
    {
        if (type != BOOLEAN)
        {
            throw std::runtime_error("Invalid bool literal type");
        }
        return boolVal;
    }

    void* getPointerValue() const
    {
        if (type != POINTER)
        {
            throw std::runtime_error("Invalid pointer literal type");
        }
        return pointerVal;
    }

    void setPointerValue(void *value)
    {
        if (type != POINTER)
        {
            throw std::runtime_error("Invalid pointer literal type");
        }
        pointerVal = value;
    }

    void setStringValue(const std::string &value)
    {
        if (type != STRING)
        {
            throw std::runtime_error("Invalid string literal type");
        }
        free(stringVal);
        stringVal = strdup(value.c_str());
    }

    void setIntValue(int value)
    {
        if (type != INT)
        {
            throw std::runtime_error("Invalid literal type");
        }
        intVal = value;
    }

    void setFloatValue(float value)
    {
        if (type != FLOAT)
        {
            throw std::runtime_error("Invalid float literal type");
        }
        floatVal = value;
    }
    void setBoolValue(bool value)
    {
        if (type != BOOLEAN)
        {
            throw std::runtime_error("Invalid bool literal type");
        }
        boolVal = value;
    }
    

   

   

 
    
    
};