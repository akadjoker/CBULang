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
    BYTE,
    BOOLEAN,
    UNDEFINED
};

class Literal
{
private:
    LiteralType type;
     using LiteralValue = std::variant<double, long, bool, unsigned char, std::string>;
     LiteralValue value;


public:
    Literal();
    Literal(long v);
    Literal(double v);
    Literal(bool v);
    Literal(unsigned char v);
    Literal(const std::string &v);
    ~Literal();


 
    std::string     getString() const;
    double          getFloat() const;
    bool            getBool() const;
    unsigned char   getByte() const;
    long            getInt() const;

    void setString(const std::string &v);
    void setFloat(const double &v);
    void setBool(const bool &v);
    void setByte(const unsigned char &v);
    void setInt(const long &v);

    bool isTruthy() const;
    bool isEqual(const Literal &other) const;
    bool isEqual( Literal *other) const;

    bool isInt() const { return type == INT; }
    bool isFloat() const { return type == FLOAT; }
    bool isByte() const { return type == BYTE; }
    bool isBool() const { return type == BOOLEAN; }
    bool isString() const { return type == STRING; }



    long            asInt() const;
    std::string     asString() const;
    bool            asBool() const;
    unsigned char   asByte() const;
    double          asFloat() const;

    bool assign(const Literal &other);
    bool assign(Literal *other);

    void print();

    LiteralType getType() const { return type; }


    std::string toString() const;
};


using LiteralPtr = std::shared_ptr<Literal>;

// class List 
// {

// private:
//     LiteralType type;
//     std::vector<LiteralPtr> elements;
// public:
//     List(LiteralType type);

//     LiteralType getType() const { return type; }
//     void add(const LiteralPtr &element) ;
//     LiteralPtr get(size_t index);
//     void set(size_t index, LiteralPtr element);
//     void clear();
//     bool remove(size_t index);
//     LiteralPtr pop();
//     size_t size() const { return elements.size(); }
// };

// class Map 
// {
//     private:
//     std::unordered_map<std::string, LiteralPtr> entries;
//     LiteralType type;
// public:
//     Map(LiteralType type);
    
//     LiteralType getType() const { return type; }
//     void set(const std::string &key, const LiteralPtr &value);
//     LiteralPtr get(const std::string &key);
//     void clear();
//     bool remove(const std::string &key);
//     bool contains(const std::string &key);
//     size_t size() const { return entries.size(); }
   
// };