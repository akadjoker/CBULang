#include "pch.h"
#include "Literal.hpp"
#include "Interpreter.hpp"
#include "Utils.hpp"


Literal::Value::Value() 
{
    stringValue=nullptr;
}

Literal::Value::~Value() {}

Literal::~Literal()
{
    
//Log(0, "Delete Literal: %s", toString().c_str());
clear();

}

void Literal::clear()
{
    if (type == STRING)
    {
        std::free(value.stringValue);
        value.stringValue = nullptr;
    }
}


bool Literal::isTrue() const
{
    
    if (type == INT)
        return value.intValue != 0;
    else if (type == FLOAT)
        return value.floatValue != 0.0f;
    else if (type == BOOLEAN)
        return value.boolValue;
    
    return false;
}

Literal::Literal(const Literal &other) : type(other.type)
{
    copyValue(other);
}

Literal &Literal::operator=(const Literal &other)
{
    if (this != &other)
    {
        clear();
        type = other.type;
        copyValue(other);
    }
    return *this;
}

void Literal::copyValue(const Literal &other)
{

 //  Log(0, "Copy Literal: %s", other.toString().c_str());

switch (other.type)
{
case LiteralType::INT:
    value.intValue = other.value.intValue;
    break;
case LiteralType::BOOLEAN:
    value.boolValue = other.value.boolValue;
    break;
case LiteralType::FLOAT:
    value.floatValue = other.value.floatValue;
    break;
case LiteralType::STRING:
    {
        if (value.stringValue != nullptr && other.value.stringValue != nullptr)
        {
            std::free(value.stringValue);
            value.stringValue = strdup(other.value.stringValue);
        } else 
        {
            if (other.value.stringValue != nullptr)
                value.stringValue = strdup(other.value.stringValue);
            else
                value.stringValue = strdup("NULL");
        }
    }
    break;
case LiteralType::POINTER:
    value.pointerValue = other.value.pointerValue;
    break;

}
}

std::string Literal::toString() const
{


    switch (type)
    {
    case STRING:
        if (value.stringValue == nullptr)
            return "String: null";
        return "String: " + std::string(value.stringValue);
    case INT:
        return "Int: " + std::to_string(value.intValue);
    case FLOAT:
        return "Float: " + std::to_string(value.floatValue);
    case BOOLEAN:
    {
        if (value.boolValue)
            return "Boolean: true";
        else
            return "Boolean: false";
    }
    case POINTER:
    {
        if (value.pointerValue == nullptr)
            return "Pointer: nil";
        else
        {
            int ref = *(int *)value.pointerValue;
            return "Pointer" + std::to_string(ref);
        }
    }
    }
    return "Unknown";
}

Literal::Literal()
{
    type = INT;
    value.intValue = 0;
}

Literal::Literal(int value)
{
    type = INT;
    this->value.intValue = value;
}

Literal::Literal(double value)
{
    type = FLOAT;
    this->value.floatValue = value;
}

Literal::Literal(bool value)
{
    type = BOOLEAN;
    this->value.boolValue = value;
}

Literal::Literal(const char *stringValue)
{
    type = STRING;
    if (stringValue == nullptr)
    {
         
        value.stringValue = strdup("NULL");
        return;
    }
    
    value.stringValue = strdup(stringValue);
    
}

Literal::Literal(void *pointerValue)
{
    type = POINTER;
    this->value.pointerValue = pointerValue;
}


char *Literal::getString() const
{
    if (type != STRING)
    {
        std::cout << "Invalid string literal type" << std::endl;
        return nullptr;
    }
  
    return value.stringValue;
}

int Literal::getInt() const
{
    
    return value.intValue;
}

double Literal::getFloat() const
{
   
    return value.floatValue;
}

bool Literal::getBool() const
{
   
    return value.boolValue;
}

void *Literal::getPointer() const
{
    if (type != POINTER)
    {
        std::cout << "Invalid pointer literal type" << std::endl;
        return nullptr;
    }
    return value.pointerValue;
}

void Literal::setPointer(void *value)
{
  
    type = POINTER;
    this->value.pointerValue = value;
}

void Literal::setString(const char *stringValue)
{
   
    type = STRING;
    if (value.stringValue != nullptr)
    {
        std::free(value.stringValue);
    }
    value.stringValue = strdup(stringValue);
}

void Literal::setInt(int value)
{

    type = INT;
    this->value.intValue = value;
}

void Literal::setFloat(double value)
{

    type = FLOAT;
    this->value.floatValue = value;
}

void Literal::setBool(bool value)
{

    type = BOOLEAN;
    this->value.boolValue = value;
}

bool Literal::copyFrom(const Literal *other)
{
    if (!other)
    {
        return false;
    }
    type = other->type;
    copyValue(*other);
    return true;
    
}
