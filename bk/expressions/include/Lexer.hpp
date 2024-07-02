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
    std::vector<Token> tokens;
    std::unordered_map<std::string, TokenType> keywords;

    char peek();
    char advance();
    bool match(char expected);
    char peekNext();

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

public:
    Lexer(const std::string &input);
    ~Lexer() = default;
    void scanToken();
    std::vector<Token> scanTokens();
};