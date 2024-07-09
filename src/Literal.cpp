#include "pch.h"
#include "Literal.hpp"
#include "Interpreter.hpp"
#include "Utils.hpp"





// List::List(LiteralType type)
// {
//     this->type = type;
// }

// void List::add(const LiteralPtr &element)
// {
//     if (element == nullptr )        return;
//     elements.push_back(element);
// }

// LiteralPtr List::get(size_t index)
// {
//     if (index >= elements.size())
//         return nullptr;
//     return elements[index];
// }

// void List::set(size_t index, LiteralPtr element)
// {
//     if (index >= elements.size())
//         return;
//     elements[index] = element;
// }

// void List::clear()
// {
//     elements.clear();
// }

// bool List::remove(size_t index)
// {
//     if (index >= elements.size())
//         return false;
//     elements.erase(elements.begin() + index);
//     return true;
// }



// LiteralPtr List::pop()
// {
//     if (elements.size() == 0)
//         return nullptr;
//     LiteralPtr ret = elements.back();
//     elements.pop_back();
//     return ret;
// }
// ///////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////
// Map::Map(LiteralType type)
// {
//     this->type = type;
// }

// void Map::set(const std::string &key, const LiteralPtr &value)
// {
//     entries[key] = value;
// }

// LiteralPtr Map::get(const std::string &key)
// {
//     if (entries.find(key) == entries.end())
//         return nullptr;
//     return entries[key];
// }

// void Map::clear()
// {
//     entries.clear();
// }

// bool Map::remove(const std::string &key)
// {
//     if (entries.find(key) == entries.end())
//         return false;
//     entries.erase(key);
//     return true;
// }

// bool Map::contains(const std::string &key)
// {
//     return entries.find(key) != entries.end();
// }

// Literal::Literal()
// {
//     type = UNDEFINED;
//     value = 0;
// }

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

Literal::Literal()
{
    type = UNDEFINED;
    value = 0;
}

Literal::Literal(long v)
{
    type = INT;
    value = v;
}

Literal::Literal(double v)
{
    type = FLOAT;
    value = v;
}

Literal::Literal(bool v)
{
    type = BOOLEAN;
    value = v;
}

Literal::Literal(unsigned char v)
{
    type = BYTE;
    value = v;
}

Literal::Literal(const std::string &v)
{
    type = STRING;
    value = v;
}

Literal::~Literal()
{
   // Log(0, "Literal deleted %s", toString().c_str());
}

std::string Literal::getString() const
{
    if (type == STRING)
        return std::get<std::string>(value);
    Log(1, "Literal is not a string (%s)",toString().c_str()); 
    return "null";
}

double Literal::getFloat() const
{
    if (type == FLOAT)
        return std::get<double>(value);
    if (type == INT)
        return static_cast<double>(getInt());
    if (type == BYTE)
        return static_cast<double>(getByte());
    if (type == BOOLEAN)
        return static_cast<double>(getBool());
    return -1.0;
}

bool Literal::getBool() const
{
    if (type == BOOLEAN)
        return std::get<bool>(value);
    
    if (type == INT)
        return static_cast<bool>(getInt());
    if (type == BYTE)
        return static_cast<bool>(getByte());
    if (type == FLOAT)
        return static_cast<bool>(getFloat());
  
    return false;
}

unsigned char Literal::getByte() const
{
    if (type == BYTE)
        return std::get<unsigned char>(value);
    if (type == INT)
        return static_cast<unsigned char>(getInt());
    if (type == FLOAT)
        return static_cast<unsigned char>(getFloat());
    if (type == BOOLEAN)
        return static_cast<unsigned char>(getBool());
    return 255;
   
}

long Literal::getInt() const
{
    if (type == INT)
        return std::get<long>(value);
    if (type == FLOAT)
        return static_cast<long>(getFloat());
   else if (type == BYTE)
        return static_cast<long>(getByte());
    else if (type == BOOLEAN)
        return static_cast<long>(getBool());

    return INT32_MAX;
}

void Literal::setString(const std::string &v)
{
    if (type == STRING)
        value = v;
    else
    {
        if (type == INT)
            value = static_cast<long>(std::stol(v)); 
        else if (type == FLOAT)
            value = static_cast<double>(std::stold(v));
        else if (type == BYTE)
            value =  static_cast<unsigned char>(std::stoul(v));
        else if (type == BOOLEAN)
            value = static_cast<bool>(std::stoi(v));
         else
         {
                type = STRING;
                value = v;
         }   
    }

  

}

void Literal::setFloat(const double &v)
{
    if (type == FLOAT)
        value = v;
    else
    {
        if (type == INT)
            value = static_cast<double>(v);
        else if (type == BOOLEAN)
            value = static_cast<bool>(v);
       else  if (type == BYTE)
            value =  static_cast<unsigned char>(v);
        else if (type == STRING)
            value = std::to_string(v);
        {
            type = FLOAT;
            value = v;
        }
            
    }


}

void Literal::setBool(const bool &v)
{
    if (type == BOOLEAN)
        value = v;
    else
    {
        if (type == INT)
            value = static_cast<long>(v);
        else if (type == FLOAT)
            value = static_cast<double>(v);
        else if (type == BYTE)
            value =  static_cast<unsigned char>(v);
        else if (type == STRING)
            value = std::to_string(v);
        else 
        {
            type = BOOLEAN;
            value = v;
        }
    }

}

void Literal::setByte(const unsigned char &v)
{
    if (type == BYTE)
        value = v;
    else
    {
        if (type == INT)
            value = static_cast<long>(v);
        else if (type == FLOAT)
            value = static_cast<double>(v);
        else if (type == BOOLEAN)
            value = static_cast<bool>(v);
        else if (type == STRING)
            value = std::to_string(v);
        else
        {
            type = BYTE;
            value = v;
        }
    }

}

void Literal::setInt(const long &v)
{
    if (type == INT)
        value = v;
    else
    {
        if (type == FLOAT)
            value = static_cast<double>(v);
        else if (type == BOOLEAN)
            value = static_cast<bool>(v);
        else if (type == BYTE)
            value =  static_cast<unsigned char>(v);
        else if (type == STRING)
            value = std::to_string(v);
        else 
        {
            type = INT;
            value = v;
        }
    }
  
}

bool Literal::isTruthy() const
{
    if (type == BOOLEAN)
        return std::get<bool>(value);
    if (type == INT)
        return std::get<long>(value) != 0;
    if (type == FLOAT)
        return std::get<double>(value) != 0.0;
    if (type == BYTE)
        return std::get<unsigned char>(value) != 0;
    if (type == STRING)
        return std::get<std::string>(value) != "";
    return false;
}

bool Literal::isEqual(const Literal &other) const
{
    if (type == other.type)
    {
        if (type == INT)
            return std::get<long>(value) == std::get<long>(other.value);
        if (type == FLOAT)
            return std::get<double>(value) == std::get<double>(other.value);
        if (type == BYTE)
            return std::get<unsigned char>(value) == std::get<unsigned char>(other.value);
        if (type == BOOLEAN)
            return std::get<bool>(value) == std::get<bool>(other.value);
        if (type == STRING)
            return std::get<std::string>(value) == std::get<std::string>(other.value);
    } else 
    {
        if (type == INT && other.type == FLOAT)
            return static_cast<double>(std::get<long>(value)) == std::get<double>(other.value);
        if (type == FLOAT && other.type == INT)
            return std::get<double>(value) == static_cast<double>(std::get<long>(other.value));
        if (type == INT && other.type == BYTE)
            return static_cast<unsigned char>(std::get<long>(value)) == std::get<unsigned char>(other.value);
        if (type == BYTE && other.type == INT)
            return std::get<unsigned char>(value) == static_cast<unsigned char>(std::get<long>(other.value));
        if (type == INT && other.type == BOOLEAN)
            return static_cast<bool>(std::get<long>(value)) == std::get<bool>(other.value);
        if (type == BOOLEAN && other.type == INT)
            return std::get<bool>(value) == static_cast<bool>(std::get<long>(other.value)); 
         
    }
    return false;
}

bool Literal::isEqual(Literal *other) const
{
    return isEqual(*other);
}


long Literal::asInt() const
{
    if (type == INT)
        return std::get<long>(value);
    if (type == FLOAT)
        return static_cast<long>(std::get<double>(value));
    if (type == BYTE)
        return static_cast<long>(std::get<unsigned char>(value));
    if (type == BOOLEAN)
        return static_cast<long>(std::get<bool>(value));
    return 0;
}

double Literal::asFloat() const
{
    if (type == INT)
        return static_cast<double>(std::get<long>(value));
    if (type == FLOAT)
        return std::get<double>(value);
    if (type == BYTE)
        return static_cast<double>(std::get<unsigned char>(value));
    if (type == BOOLEAN)
        return static_cast<double>(std::get<bool>(value));
    return 0.0;
}

bool Literal::assign(const Literal &other)
{


    auto otherType = other.getType();

    // Handle assignment between the same types
    if (type == otherType)
    {
        value = other.value;
        return true;
    }

    // Handle conversions between different types
    if (type == INT)
    {
        if (otherType == FLOAT)
        {
            value = static_cast<long>(other.getFloat());
            return true;
        }
        if (otherType == BYTE)
        {
            value = static_cast<long>(other.getByte());
            return true;
        }
        if (otherType == BOOLEAN)
        {
            value = static_cast<long>(other.getBool());
            return true;
        }
    }
    else if (type == FLOAT)
    {
        if (otherType == INT)
        {
            value =  static_cast<double>(other.getInt());
            return true;
        }
        if (otherType == BYTE)
        {
            value = static_cast<double>(other.getByte());
            return true;
        }
        if (otherType == BOOLEAN)
        {
            value = static_cast<double>(other.getBool());
            return true;
        }
    }
    else if (type == BYTE)
    {
        if (otherType == INT)
        {
            value = static_cast<unsigned char>(other.getInt());
            return true;
        }
        if (otherType == FLOAT)
        {
            value = static_cast<unsigned char>(other.getFloat());
            return true;
        }
        if (otherType == BOOLEAN)
        {
            value = static_cast<unsigned char>(other.getBool());
            return true;
        }
    }
    else if (type == BOOLEAN)
    {
        if (otherType == INT)
        {
            value = static_cast<bool>(other.getInt());
            return true;
        }
        if (otherType == FLOAT)
        {
            value = static_cast<bool>(other.getFloat());
            return true;
        }
        if (otherType == BYTE)
        {
            value = static_cast<bool>(other.getByte());
            return true;
        }
    }

    Log(2, "Literal::assign: unsupported type conversion");
    return false;
}




bool Literal::assign(Literal *other)
{
    if (!other)
    {
        Log(2, "Literal::assign: other is null");
        return false;
    }

    auto otherType = other->getType();

    // Handle assignment between the same types
    if (type == otherType)
    {
        value = other->value;
        return true;
    }

    // Handle conversions between different types
    if (type == INT)
    {
        if (otherType == FLOAT)
        {
            value = static_cast<long>(other->getFloat());
            return true;
        }
        if (otherType == BYTE)
        {
            value = static_cast<long>(other->getByte());
            return true;
        }
        if (otherType == BOOLEAN)
        {
            value = static_cast<long>(other->getBool());
            return true;
        }
    }
    else if (type == FLOAT)
    {
        if (otherType == INT)
        {
            value = static_cast<double>(other->getInt());
            return true;
        }
        if (otherType == BYTE)
        {
            value = static_cast<double>(other->getByte());
            return true;
        }
        if (otherType == BOOLEAN)
        {
            value = static_cast<double>(other->getBool());
            return true;
        }
    }
    else if (type == BYTE)
    {
        if (otherType == INT)
        {
            value = static_cast<unsigned char>(other->getInt());
            return true;
        }
        if (otherType == FLOAT)
        {
            value = static_cast<unsigned char>(other->getFloat());
            return true;
        }
        if (otherType == BOOLEAN)
        {
            value = static_cast<unsigned char>(other->getBool());
            return true;
        }
    }
    else if (type == BOOLEAN)
    {
        if (otherType == INT)
        {
            value = static_cast<bool>(other->getInt());
            return true;
        }
        if (otherType == FLOAT)
        {
            value = static_cast<bool>(other->getFloat());
            return true;
        }
        if (otherType == BYTE)
        {
            value = static_cast<bool>(other->getByte());
            return true;
        }
    }

    Log(2, "Literal::assign: unsupported type conversion ");
    return false;
}

    


void Literal::print()
{
    if (type == INT)
        Log(3, "%ld", std::get<long>(value));
    if (type == FLOAT)
        Log(3, "%f", std::get<double>(value));
    if (type == BYTE)
        Log(3, "%u", std::get<unsigned char>(value));
    if (type == BOOLEAN)
        Log(3, "%s", std::get<bool>(value) ? "true" : "false");
    if (type == STRING)
        Log(3, "%s", std::get<std::string>(value).c_str());

}

unsigned char Literal::asByte() const
{
    if (type == INT)
        return static_cast<unsigned char>(std::get<long>(value));
    if (type == FLOAT)
        return static_cast<unsigned char>(std::get<double>(value));
    if (type == BYTE)
        return std::get<unsigned char>(value);
    if (type == BOOLEAN)
        return static_cast<unsigned char>(std::get<bool>(value));
    return 0;
}

bool Literal::asBool() const
{
    if (type == INT)
        return std::get<long>(value) != 0;
    if (type == FLOAT)
        return std::get<double>(value) != 0.0;
    if (type == BYTE)
        return std::get<unsigned char>(value) != 0;
    if (type == BOOLEAN)
        return std::get<bool>(value);
    return false;
}

std::string Literal::asString() const
{
    if (type == STRING)
        return std::get<std::string>(value);
    if (type == INT)
        return std::to_string(std::get<long>(value));
    if (type == FLOAT)
        return std::to_string(std::get<double>(value));
    if (type == BYTE)
        return std::to_string(std::get<unsigned char>(value));
    if (type == BOOLEAN)
        return std::to_string(std::get<bool>(value));
    return "null";
}


std::string Literal::toString() const
{
    if (type == INT)
        return "INT: "+std::to_string(std::get<long>(value));
    if (type == FLOAT)
        return "FLOAT: "+std::to_string(std::get<double>(value));
    if (type == BYTE)
        return "BYTE: "+std::to_string(std::get<unsigned char>(value));
    if (type == BOOLEAN)
        return "BOOLEAN: "+std::to_string(std::get<bool>(value));
    if (type == STRING)
        return "STRING: "+std::get<std::string>(value);
    return "LITERAL UNKNOWN";  
}
