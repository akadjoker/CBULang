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
    LiteralType type;
      union Value
    {
        int intValue;
        bool boolValue;
        double floatValue;
        char *stringValue;
        void *pointerValue;

        Value();
        ~Value();
    } value;

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

    Literal(const Literal &other);
    Literal &operator=(const Literal &other);
    void copyValue(const Literal &other);
public:
    Literal();
    Literal(int ivalue);
    Literal(bool bvalue);
    Literal(double fvalue);
    Literal(const char *stringValue);
    Literal(void *pointerValue);

    virtual ~Literal();

  

    std::string toString() const;

    LiteralType getType() const { return type; }

    char *getString() const;
    int   getInt() const;
    double getFloat() const;
    bool  getBool() const;
    void *getPointer() const;

    void setPointer(void *value);
    void setString(const char *stringValue);
    void setInt(int value);
    void setFloat(double value);
    void setBool(bool value);

    bool isTrue() const;

    bool copyFrom(const Literal *other);

    void clear();
};