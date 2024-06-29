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
            throw std::runtime_error("Invalid literal type");
        }
        return stringVal;
    }

    int getIntValue() const
    {
        if (type != INT)
        {
            throw std::runtime_error("Invalid literal type");
        }
        return intVal;
    }

    float getFloatValue() const
    {
        if (type != FLOAT)
        {
            throw std::runtime_error("Invalid literal type");
        }
        return floatVal;
    }

    bool getBoolValue() const
    {
        if (type != BOOLEAN)
        {
            throw std::runtime_error("Invalid literal type");
        }
        return boolVal;
    }

    void* getPointerValue() const
    {
        if (type != POINTER)
        {
            throw std::runtime_error("Invalid literal type");
        }
        return pointerVal;
    }

    //operatores 

    // bool operator==(const Literal &other) const
    // {
    //     if (type != other.type)
    //     {
    //         std::string text = "Try to compare literals of different types: " + ltString(type) + " vs " + ltString(other.type);
    //         throw std::runtime_error(text);
    //     }
    //     switch (type)
    //     {
    //         case STRING:
    //             return stringVal == other.stringVal;
    //         case INT:
    //             return intVal == other.intVal;
    //         case FLOAT:
    //             return floatVal == other.floatVal;
    //     }
    //     return false;
    // }

    // bool operator!=(const Literal &other) const
    // {
    //     if (type != other.type)
    //     {
    //         std::string text = "Try to compare literals of different types: " + ltString(type) + " vs " + ltString(other.type);
    //         throw std::runtime_error(text);
    //         return true;
    //     }
    //     return !(*this == other);
    // }

    // bool operator<(const Literal &other) const
    // {
    //     if (type != other.type)
    //     {
    //         std::string text = "Try to compare literals of different types: " + ltString(type) + " vs " + ltString(other.type);
    //         throw std::runtime_error(text);
    //         return true;
    //     }
    //     switch (type)
    //     {
    //         case STRING:
    //             return stringVal < other.stringVal;
    //         case INT:
    //             return intVal < other.intVal;
    //         case FLOAT:
    //             return floatVal < other.floatVal;
    //     }
    //     return false;
    // }

    // bool operator>(const Literal &other) const
    // {
    //     if (type != other.type)
    //     {
    //         std::string text = "Try to compare literals of different types: " + ltString(type) + " vs " + ltString(other.type);
    //         throw std::runtime_error(text);
    //         return true;
    //     }
    //     return !(*this < other);
    // }

    // bool operator<=(const Literal &other) const
    // {
    //     if (type != other.type)
    //     {
    //         std::string text = "Try to compare literals of different types: " + ltString(type) + " vs " + ltString(other.type);
    //         throw std::runtime_error(text);
    //         return true;
    //     }
    //     return *this < other || *this == other;
    // }

    // bool operator>=(const Literal &other) const
    // {
    //     if (type != other.type)
    //     {
    //         std::string text = "Try to compare literals of different types: " + ltString(type) + " vs " + ltString(other.type);
    //         throw std::runtime_error(text);
    //         return true;
    //     }
    //     return *this > other || *this == other;
    // }

    // bool operator!() const
    // {
    //     if (type == STRING)
    //     {
    //         return stringVal.empty();
    //     }
    //     if (type == INT)
    //     {
    //         return intVal == 0;
    //     }
    //     if (type == FLOAT)
    //     {
    //         return floatVal == 0.0;
    //     }
    //     return false;
    // }

    // Literal operator+(const Literal &other) const
    // {

    //     if (type == STRING && other.type == STRING)
    //     {
    //         return Literal(stringVal + other.stringVal);
    //     }
    //     if (type == INT && other.type == INT)
    //     {
    //         return Literal(intVal + other.intVal);
    //     }
    //     if (type == FLOAT && other.type == FLOAT)
    //     {
    //         return Literal(floatVal + other.floatVal);
    //     }
    //     if (type == INT && other.type == FLOAT)
    //     {
    //        return Literal(intVal + other.floatVal);
    //     }
    //     if (type == FLOAT && other.type == INT)
    //     {
    //         return Literal(floatVal + static_cast<float>(other.intVal));
    //     }
    //     if (type == STRING && other.type == INT)
    //     {
    //         return Literal(stringVal + std::to_string(other.intVal));
    //     }
    //     if (type == INT && other.type == STRING)
    //     {
    //         return Literal(std::to_string(intVal) + other.stringVal);
    //     }
    //     if (type == STRING && other.type == FLOAT)
    //     {
    //         return Literal(stringVal + std::to_string(other.floatVal));
    //     }
    //     if (type == FLOAT && other.type == STRING)
    //     {
    //         return Literal(std::to_string(floatVal) + other.stringVal);
    //     }
        
       
    // }

    // Literal operator-(const Literal &other) const
    // {
    //     if (type == INT && other.type == INT)
    //     {
    //         return Literal(intVal - other.intVal);
    //     }
    //     if (type == FLOAT && other.type == FLOAT)
    //     {
    //         return Literal(floatVal - other.floatVal);
    //     }
    //     if (type == INT && other.type == FLOAT)
    //     {
    //         return Literal(intVal - other.floatVal);
    //     }
    //     if (type == FLOAT && other.type == INT)
    //     {
    //         return Literal(floatVal - static_cast<float>(other.intVal));
    //     }

    //     if (type == INT && other.type == STRING)
    //     {
    //         return Literal(intVal - std::stoi(other.stringVal));
    //     }
    //     if (type == STRING && other.type == INT)
    //     {
    //         return Literal(std::stoi(stringVal) - intVal);
    //     }

    //      if (type == STRING || other.type == STRING)
    //     {
    //         throw std::runtime_error("Cannot subtract strings");
    //     }

    // }

    // Literal operator*(const Literal &other) const
    // {
    //     if (type == INT && other.type == INT)
    //     {
    //         return Literal(intVal * other.intVal);
    //     }
    //     if (type == FLOAT && other.type == FLOAT)
    //     {
    //         return Literal(floatVal * other.floatVal);
    //     }
    //     if (type == INT && other.type == FLOAT)
    //     {
    //         return Literal(floatVal * other.intVal);
    //     }
    //     if (type == FLOAT && other.type == INT)
    //     {
    //         return Literal(floatVal * static_cast<float>(other.intVal));
    //     }

    //     if (type == STRING || other.type == STRING)
    //     {
    //         throw std::runtime_error("Cannot multiply strings");
    //     }
    // }


    //  Literal operator+=(const Literal &other)
    // {

    //     if (type == INT && other.type == INT)
    //     {
    //         return Literal(intVal + other.intVal);
    //     }
    //     if (type == FLOAT && other.type == FLOAT)
    //     {
    //         return Literal(floatVal + other.floatVal);
    //     }
    //     if (type == INT && other.type == FLOAT)
    //     {
    //         return Literal(intVal + other.floatVal);
    //     }
    //     if (type == FLOAT && other.type == INT)
    //     {
    //         return Literal(floatVal + static_cast<float>(other.intVal));
    //     }

    //     if (type == STRING || other.type == STRING)
    //     {
    //         return Literal(stringVal + other.stringVal);
    //     }

    //     if (type == INT && other.type == STRING)
    //     {
    //         throw std::runtime_error("Cannot add string to int");
    //     }
    //     if (type == FLOAT && other.type == STRING)
    //     {
    //         throw std::runtime_error("Cannot add string to float");
    //     }
    //     if (type == STRING && other.type == INT)
    //     {
    //         return Literal(std::stoi(stringVal) + intVal);
    //     }
    //     if (type == STRING && other.type == FLOAT)
    //     {
    //         return Literal(std::to_string(floatVal) + other.stringVal);
    //     }
       
    // }

   

   

 
    
    
};