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
    
    if (type == NUMBER)
        return value.numberValue != 0.0;
    else if (type == BOOLEAN)
        return value.boolValue;
    else if (type == STRING)
        return value.stringValue != nullptr;
    
    return false;
}

bool Literal::isEqual(const Literal *other) const
{

    if (type != other->type)
        return false;
    if (type == LiteralType::STRING)
        return value.stringValue == other->value.stringValue;
    else if (type == LiteralType::NUMBER)
        return value.numberValue == other->value.numberValue;
    else if (type == LiteralType::BOOLEAN)
        return value.boolValue == other->value.boolValue;
    return false;
}


bool Literal::isNumber() const
{
    return type == LiteralType::NUMBER;
}

bool Literal::isBool() const
{
    return type == LiteralType::STRING;
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
    case LiteralType::BOOLEAN:
        value.boolValue = other.value.boolValue;
        break;
    case LiteralType::NUMBER:
        value.numberValue = other.value.numberValue;
        break;
    case LiteralType::STRING:
        {
            if (value.stringValue != nullptr )
            {
                std::free(value.stringValue);
            }
            if (other.value.stringValue != nullptr)
                    value.stringValue = strdup(other.value.stringValue);
            else
                    value.stringValue = strdup("NULL");
            
        }
        break;
        }
}



std::string Literal::toString() const
{


    switch (type)
    {
        case STRING:
        {
            if (value.stringValue == nullptr)
                return "String: null";
            return "String: " + std::string(value.stringValue);
        }
        case NUMBER:
            return "number: " + std::to_string(value.numberValue);
        case BOOLEAN:
        {
            if (value.boolValue)
                return "Boolean: true";

            return "Boolean: false";
        }
    }
    return "Unknown";
}

Literal::Literal()
{
    type = NUMBER;
    value.numberValue = 0.0;
}


Literal::Literal(double value)
{
    type = NUMBER;
    this->value.numberValue = value;
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


char *Literal::getString() const
{
    if (type != STRING)
    {
        std::cout << "Invalid string literal type" << std::endl;
        return nullptr;
    }
  
    return value.stringValue;
}


double Literal::getNumber() const
{
   
    return value.numberValue;
}

bool Literal::getBool() const
{
    return value.boolValue;
}

int Literal::getInt() const
{
    int result = (int)value.numberValue;
    return result;
}

float Literal::geFloat() const
{
    float result = (float)value.numberValue;
    return result;
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

void Literal::setNumber(double value)
{
    type = NUMBER;
    this->value.numberValue = value;
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
    if (element == nullptr )        return;
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
