#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Token.hpp"

enum LiteralType
{
    STRING,
    NUMBER,
    BOOLEAN
};

class Literal
{
private:
    LiteralType type;
      union Value
    {
        bool boolValue;
        double numberValue;
        char *stringValue;

        Value();
        ~Value();
    } value;

    std::string ltString(LiteralType type) const
    {
        switch (type)
        {
        case STRING:
            return "String";
        case NUMBER:
            return "Number";
        case BOOLEAN:
            return "Boolean";
        }
        return "";
    }

    Literal(const Literal &other);
    Literal &operator=(const Literal &other);
    void copyValue(const Literal &other);
public:
    Literal();
    Literal(bool bvalue);
    Literal(double fvalue);
    Literal(const char *stringValue);
   
    virtual ~Literal();

  

    std::string toString() const;

    LiteralType getType() const { return type; }

    char *getString() const;
    double getNumber() const;
    bool  getBool() const;
    int   getInt() const;
    float geFloat() const;
    
    void setString(const char *stringValue);
    void setNumber(double value);
    void setBool(bool value);

    bool isTrue() const;
    bool isEqual(const Literal *other) const;

    bool isNumber() const;
    bool isBool() const;
    bool isString() const;
    
    bool copyFrom(const Literal *other);

    void clear();




    
};
using LiteralPtr = std::shared_ptr<Literal>;

class List 
{

private:
    LiteralType type;
    std::vector<LiteralPtr> elements;
public:
    List(LiteralType type);

    LiteralType getType() const { return type; }
    void add(const LiteralPtr &element) ;
    LiteralPtr get(size_t index);
    void set(size_t index, LiteralPtr element);
    void clear();
    bool remove(size_t index);
    LiteralPtr pop();
    size_t size() const { return elements.size(); }
};

class Map 
{
    private:
    std::unordered_map<std::string, LiteralPtr> entries;
    LiteralType type;
public:
    Map(LiteralType type);
    
    LiteralType getType() const { return type; }
    void set(const std::string &key, const LiteralPtr &value);
    LiteralPtr get(const std::string &key);
    void clear();
    bool remove(const std::string &key);
    bool contains(const std::string &key);
    size_t size() const { return entries.size(); }
   
};