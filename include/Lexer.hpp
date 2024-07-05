#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Token.hpp"

class Lexer
{
private:
    std::string input;
    int start;
    int current;
    int line;
    bool panicMode;
    bool isProcedure ;
    bool isFunction ;
    bool isProcess ;
    std::vector<Token> tokens;
    std::unordered_map<std::string, TokenType> keywords;
    std::unordered_map<std::string, int> procedures;
    std::unordered_map<std::string, int> functions;
    std::unordered_map<std::string, int> processes;
    std::stack<int> blocks;//begin end
    std::stack<int> brackets;  // []
    std::stack<int> braces;   // {}
    std::stack<int> parens;   // ()


    

    char peek();
    char advance();
    bool match(char expected);
    char peekNext();
    char peekAhead(int n);
    char previous();

    bool isAtEnd();
    bool isDigit(char c);
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
    std::string toLower(const std::string &str);

    void identifier();
    void number();
    void string();

    
    void addToken(TokenType type, const std::string &literal="");

    void Error(std::string message);
    void Warning(std::string message);

    std::string extractIdentifier(const std::string &str);

    bool hasProcedure(const std::string &str);

    bool hasFunction(const std::string &str);

    bool hasProcess(const std::string &str);

public:
    Lexer(const std::string &input);
    ~Lexer() = default;
    void scanToken();
    bool ready();
    std::vector<Token> scanTokens();
};