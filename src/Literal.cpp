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
    else if (type == STRING)
        return value.stringValue != nullptr;
    else if (type == POINTER)
        return value.pointerValue != nullptr;
    
    return false;
}

bool Literal::isEqual(const Literal *other) const
{

    if (type != other->type)
        return false;
    if (type == LiteralType::STRING)
        return value.stringValue == other->value.stringValue;
    else if (type == LiteralType::FLOAT)
        return value.floatValue == other->value.floatValue;
    else if (type == LiteralType::BOOLEAN)
        return value.boolValue == other->value.boolValue;
    return false;
}

bool Literal::isInt() const
{
    return type == LiteralType::INT;
}

bool Literal::isFloat() const
{
    return type == LiteralType::FLOAT;
}

bool Literal::isBool() const
{
    return type == LiteralType::BOOLEAN;
}

bool Literal::isString() const
{
    return type == LiteralType::STRING;
}

bool Literal::isPointer() const
{
    return type == LiteralType::POINTER;
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

List::List(LiteralType type)
{
    this->type = type;
}

void List::add(const LiteralPtr &element)
{
    if (element == nullptr )
        return;
    
    elements.push_back(element);
}

LiteralPtr List::get(size_t index)
{
    if (index >= elements.size())
        return nullptr;
    return elements[index];
}

void List::set(size_t index, LiteralPtr element)
{
    if (index >= elements.size())
        return;
    elements[index] = element;
}

void List::clear()
{
    elements.clear();
}

bool List::remove(size_t index)
{
    if (index >= elements.size())
        return false;
    elements.erase(elements.begin() + index);
    return true;
}



LiteralPtr List::pop()
{
    if (elements.size() == 0)
        return nullptr;
    LiteralPtr ret = elements.back();
    elements.pop_back();
    return ret;
}

Map::Map(LiteralType type)
{
    this->type = type;
}

void Map::set(const std::string &key, const LiteralPtr &value)
{
    entries[key] = value;
}

LiteralPtr Map::get(const std::string &key)
{
    if (entries.find(key) == entries.end())
        return nullptr;
    return entries[key];
}

void Map::clear()
{
    entries.clear();
}

bool Map::remove(const std::string &key)
{
    if (entries.find(key) == entries.end())
        return false;
    entries.erase(key);
    return true;
}

bool Map::contains(const std::string &key)
{
    return entries.find(key) != entries.end();
}
