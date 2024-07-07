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
    bool isEqual(const Literal *other) const;

    bool isInt() const;
    bool isFloat() const;
    bool isBool() const;
    bool isString() const;
    bool isPointer() const;

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