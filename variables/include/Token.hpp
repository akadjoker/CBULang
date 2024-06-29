#pragma once

#include <string>

enum class TokenType
{
    // Single-character tokens.
    LEFT_PAREN,    // (
    RIGHT_PAREN,   // )
    LEFT_BRACE,    // {
    RIGHT_BRACE,   // }
    LEFT_BRACKET,  // [
    RIGHT_BRACKET, // ]

    COLON,     // :
    DOLLAR,    // $
    COMMA,     // ,
    DOT,       // .
    MINUS,     // -
    PLUS,      // +
    SEMICOLON, // ;
    SLASH,     // /
    STAR,      // *
    MOD,       // %
    POWER,     // ^

    // One or two character tokens.
    BANG,          // !
    BANG_EQUAL,    // !=
    EQUAL,         // =
    EQUAL_EQUAL,   // ==
    GREATER,       // >
    GREATER_EQUAL, // >=
    LESS,          // <
    LESS_EQUAL,    // <

    // Literals.
    IDENTIFIER,
    STRING,
    INT,
    FLOAT,
    BOOL,

    // Keywords.
    OR,
    XOR,
    NOT,
    AND,

    IF,
    ELSE,
    ELIF,

    BEGIN,
    END,
    PROGRAM,

    FUNCTION,
    PROCESS,

    NIL,
    FALSE,
    TRUE,

    // LITERAIL ID
    IDINT,
    IDFLOAT,
    IDBOOL,
    IDSTRING,
    // LOOPS
    WHILE,
    FOR,
    REPEAT,
    UNTIL,
    BREAK,
    CONTINUE,
    RETURN,

    // internals
    PRINT,
    NOW,

    ERROR,
    END_OF_FILE,
};
inline std::string tknString(TokenType type)
{
    switch (type)
    {
        case TokenType::LEFT_PAREN:    return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN:   return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE:    return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE:   return "RIGHT_BRACE";
        case TokenType::LEFT_BRACKET:  return "LEFT_BRACKET";
        case TokenType::RIGHT_BRACKET: return "RIGHT_BRACKET";
        case TokenType::COLON:         return "COLON";
        case TokenType::DOLLAR:        return "DOLLAR";
        case TokenType::COMMA:         return "COMMA";
        case TokenType::DOT:           return "DOT";
        case TokenType::MINUS:         return "MINUS";
        case TokenType::PLUS:          return "PLUS";
        case TokenType::SEMICOLON:     return "SEMICOLON";
        case TokenType::SLASH:         return "SLASH";
        case TokenType::STAR:          return "STAR";
        case TokenType::MOD:           return "MOD";
        case TokenType::POWER:         return "POWER";
        case TokenType::BANG:          return "BANG";
        case TokenType::BANG_EQUAL:    return "BANG_EQUAL";
        case TokenType::EQUAL:         return "EQUAL";
        case TokenType::EQUAL_EQUAL:   return "EQUAL_EQUAL";
        case TokenType::GREATER:       return "GREATER";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::LESS:          return "LESS";
        case TokenType::LESS_EQUAL:    return "LESS_EQUAL";
        case TokenType::IDENTIFIER:    return "IDENTIFIER";
        case TokenType::STRING:        return "STRING";
        case TokenType::INT:           return "INT";
        case TokenType::FLOAT:         return "FLOAT";
        case TokenType::BOOL:          return "BOOL";
        case TokenType::OR:            return "OR";
        case TokenType::XOR:           return "XOR";
        case TokenType::NOT:           return "NOT";
        case TokenType::AND:           return "AND";
        case TokenType::IF:            return "IF";
        case TokenType::ELSE:          return "ELSE";
        case TokenType::ELIF:          return "ELIF";
        case TokenType::BEGIN:         return "BEGIN";
        case TokenType::END:           return "END";
        case TokenType::PROGRAM:       return "PROGRAM";
        case TokenType::FUNCTION:      return "FUNCTION";
        case TokenType::PROCESS:       return "PROCESS";
        case TokenType::NIL:           return "NIL";
        case TokenType::FALSE:         return "FALSE";
        case TokenType::TRUE:          return "TRUE";
        case TokenType::IDINT:         return "IDINT";
        case TokenType::IDFLOAT:       return "IDFLOAT";
        case TokenType::IDBOOL:        return "IDBOOL";
        case TokenType::IDSTRING:      return "IDSTRING";
        case TokenType::WHILE:         return "WHILE";
        case TokenType::FOR:           return "FOR";
        case TokenType::REPEAT:        return "REPEAT";
        case TokenType::UNTIL:         return "UNTIL";
        case TokenType::BREAK:         return "BREAK";
        case TokenType::CONTINUE:      return "CONTINUE";
        case TokenType::RETURN:        return "RETURN";
        case TokenType::PRINT:         return "PRINT";
        case TokenType::NOW:           return "NOW";
        case TokenType::ERROR:         return "ERROR";
        case TokenType::END_OF_FILE:   return "END_OF_FILE";
        default:                       return "UNKNOWN";
    }
}



struct Token
{
    TokenType type;
    std::string lexeme;
    std::string literal;
    int line;

    Token(TokenType type, std::string lexeme, std::string literal, int line)
    {
        this->type = type;
        this->lexeme = lexeme;
        this->literal = literal;
        this->line = line;
    }

    std::string toString()
    {
        if (literal == "")
        {
            return " Token: '" + tknString(type) + "'";
        } else 
        {
        return " Token: '" + tknString(type) + "' Value: '" + literal + "'";
        }
    }
};