#include "pch.h"
#include "Lexer.hpp"



Lexer::Lexer(const std::string &input)
{
    this->input = input;
    start = 0;
    current = 0;
    line = 1;
    panicMode = false;
    isProcedure = false;
    isFunction = false;
    isProcess = false;

   keywords["and"] = TokenType::AND;
    keywords["or"] = TokenType::OR;
    keywords["not"] = TokenType::NOT;
    keywords["xor"] = TokenType::XOR;
    
    keywords["if"] = TokenType::IF;
    keywords["elif"] = TokenType::ELIF;
    keywords["else"] = TokenType::ELSE;

    keywords["for"] = TokenType::FOR;
    keywords["repeat"] = TokenType::REPEAT;
    keywords["while"] = TokenType::WHILE;
    keywords["until"] = TokenType::UNTIL;

    keywords["begin"] = TokenType::BEGIN;
    keywords["end"] = TokenType::END;
    keywords["program"] = TokenType::PROGRAM;

    keywords["break"] = TokenType::BREAK;
    keywords["continue"] = TokenType::CONTINUE;
    keywords["return"] = TokenType::RETURN;

    keywords["function"] = TokenType::FUNCTION;
    keywords["procedure"] = TokenType::PROCEDURE;
    keywords["process"] = TokenType::PROCESS;

    keywords["print"] = TokenType::PRINT;
    keywords["nil"] = TokenType::NIL;
    keywords["int"] = TokenType::IDINT;
    keywords["float"] = TokenType::IDFLOAT;
    keywords["string"] = TokenType::IDSTRING;
    keywords["bool"] = TokenType::IDBOOL;
    keywords["false"] = TokenType::FALSE;
    keywords["true"] = TokenType::TRUE;

    keywords["print"] = TokenType::PRINT;
    keywords["now"] = TokenType::NOW;


    


}

void Lexer::scanToken()
{
    char c = advance();
    switch (c)
    {
      case '(': addToken(TokenType::LEFT_PAREN); break;
      case ')': addToken(TokenType::RIGHT_PAREN); break;
      case '{': addToken(TokenType::LEFT_BRACE); break;
      case '}': addToken(TokenType::RIGHT_BRACE); break;
      case '[': addToken(TokenType::LEFT_BRACKET); break;
      case ']': addToken(TokenType::RIGHT_BRACKET); break;

      case ',': addToken(TokenType::COMMA); break;
      case '.': addToken(TokenType::DOT); break;
      case '-': addToken(TokenType::MINUS); break;
      case '+': addToken(TokenType::PLUS); break;
      case ';': addToken(TokenType::SEMICOLON); break;
      case ':': addToken(TokenType::COLON); break;

      case '^': addToken(TokenType::POWER); break;

      case '%': addToken(TokenType::MOD); break;

      case '*': addToken(TokenType::STAR); break; // [slash]

      case '!':
        addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
        break;
      case '=':
        addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
        break;
      case '<':
        addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
        break;
      case '>':
        addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
        break;


      case '/':
        if (match('/')) 
        {
          while (peek() != '\n' && !isAtEnd()) advance();
        } else
        if (match('*')) 
        {
            advance();
            while (!isAtEnd()) 
            {
                if (peek() == '*') break;
                if (peek() == '\n') line++;
                advance();
            }
            if (peek() == '*' && peekNext() == '/')
            {
                advance();
                advance();
                break;
            } else 
            {
                Error("Unterminated comment");
                return;
            }
        
        } 
        else 
        {
          addToken(TokenType::SLASH);
        }
        break;


      case ' ':
      case '\r':
      case '\t':
        // Ignore whitespace.
        break;

      case '\n':
        line++;
        break;
      case '"': string(); break;

      default:
        if (isDigit(c)) 
        {
          number();
        } else if (isAlpha(c)) 
        {
          identifier();
        } else 
        {
          Error("Unexpected character" );
        }

        break;

    }

}

std::vector<Token> Lexer::scanTokens()
{
    if (panicMode)
    {
        return tokens;
    }
    while (!isAtEnd())
    {
        start = current;
        scanToken();
        if (panicMode) 
        {
            tokens.clear();
            std::string error = input.substr(start, current - start);
            auto token = Token(TokenType::ERROR, "Error",error,line);
            tokens.push_back(token);
            return tokens;    
        }
    }
    auto token = Token(TokenType::END_OF_FILE, "EOF","",line);
    tokens.push_back(token);
    return tokens;
   
}

char Lexer::peek()
{
    if (isAtEnd())    return '\0';
    return input[current];
}

char Lexer::advance()
{
    return input[current++];
}

bool Lexer::match(char expected)
{
    if (isAtEnd())                    return false;
    if (input[current] != expected)   return false;
    
    current++;
    return true;
}

char Lexer::peekNext()
{
    if (current + 1 >= input.size()) return '\0';
    return input[current + 1];
}

char Lexer::peekAhead(int n)
{
    if (current + n >= input.size()) return '\0';
    return input[current + n];
}

char Lexer::previous()
{
    if (current - 1 < 0) return '\0';
    return input[current - 1];
}

bool Lexer::isAtEnd()
{
    return current >= input.size();
}

bool Lexer::isDigit(char c)
{
    return (c >= '0' && c <= '9');
   // || c == '.';
}

bool Lexer::isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || 
            (c >= 'A' && c <= 'Z') || 
            c == '_';
}

bool Lexer::isAlphaNumeric(char c)
{
    return isAlpha(c) || isDigit(c);
}

std::string Lexer::toLower(const std::string &str)
{
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

std::string Lexer::extractIdentifier(const std::string &str)
{
    std::string result;
    for (char c : str)
    {
        if (isAlpha(c) || c == '_')
        {
            result += c;
        }
    }
    return result;
}

bool Lexer::hasProcedure(const std::string &str)
{
    
    std::string lowerStr = toLower(str);
    for (auto it = procedures.begin(); it != procedures.end(); it++)
    {
        std::cout<<it->first<<std::endl;
        if (it->first == lowerStr)
        {
            return true;
        }
    }
    // if (keywords.find(lowerStr) != keywords.end())
    // {
    //      return true;
    // }
    return false;

}

void Lexer::identifier()
{
    
   
    while (isAlphaNumeric(peek())) advance();

   // std::cout<<" ,"<<peek()<<" ";


    std::string text = input.substr(start, current - start);
    text = toLower(text);
    
  
    if (keywords.find(text) != keywords.end())
    {
        TokenType type = keywords[text];


        if (text=="procedure")            isProcedure = true;
        if (text=="function")             isFunction = true;
        if (text=="process")              isProcess = true;
     

        
        addToken(type);
    }
    else
    {
        if (isProcedure)
        {
             addToken(TokenType::IDPROCEDURE, text);
             if (!hasProcedure(text))
             {
               procedures[text] = (int)tokens.size()-1;
             }
            
             isProcedure = false;
        } else if (isFunction)
        {
            addToken(TokenType::IDFUNCTION, text);
            isFunction = false;
        } else if (isProcess)
        {
            addToken(TokenType::IDPROCESS, text);
            isProcess = false;
        } else 
        {
             if (hasProcedure(text))
             {
               addToken(TokenType::IDPROCEDURE, text);
             } else 
             {
                addToken(TokenType::IDENTIFIER, text);
             }
        }
       
    }
}

void Lexer::string()
{
      while (peek() != '"' && !isAtEnd()) 
      {
         if (peek() == '\n') line++;
        advance();
    }

    if (isAtEnd()) 
    {
      Error("Unterminated string");
      return;
    }

    
    advance();

    // Trim the surrounding quotes.
    std::string text = input.substr(start+1  , (current - start)-2);
    
    addToken(TokenType::STRING, text);
    
    
         
}



void Lexer::number()
{
    std::string text = "";
    bool isFloat = false;

    while (isDigit(peek())) advance();

    if (peek() == '.' && isDigit(peekNext()))
    {
        isFloat = true;
        advance();
        while (isDigit(peek())) advance();
    }


   

    text = input.substr(start, current - start);
    addToken(isFloat ? TokenType::FLOAT : TokenType::INT, text);
 
}


void Lexer::addToken(TokenType type, const std::string &literal)
{
    std::string text = input.substr(start, current - start);
    //text = toLower(text);
    Token token = Token(type, text, literal, line);
    tokens.push_back(token);
}

void Lexer::Error(std::string message)
{
    panicMode = true;
    std::string text ="ERROR: " +message+ " at line: " +std::to_string(line);
    throw std::runtime_error(text);

    //std::cout << "ERROR: " << message<< " at line: " << line << std::endl;
    
}

void Lexer::Warning(std::string message)
{

    std::string text ="WARNING: " +message+ " at line: " +std::to_string(line);
    throw std::runtime_error(text);
    
}
