
#include "pch.h"
#include "Token.hpp"
#include "Literal.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"

void Test(const std::string &code, std::unique_ptr<Literal> expected)
{
    Lexer lexer(code);
    std::vector<Token> tokens = lexer.scanTokens();
    
    Parser parser(tokens);
    std::unique_ptr<Expr> result = parser.result();

    Interpreter interpreter;
    bool testResult = interpreter.test(std::move(result), std::move(expected));

    std::cout << "Test \"" << code << "\": " << (testResult ? "Passed" : "Failed") << std::endl;
}

int main()
{

    std::string code = R"(

         int a;
       
       
        )";

    try
    {

        Lexer lexer = Lexer(code);
        std::vector<Token> tokens = lexer.scanTokens();
        for (Token token : tokens)
        {
            std::cout << token.toString() << std::endl;
        }
        Parser parser = Parser(tokens);
        std::unique_ptr<Expr> result = parser.result();

        // Printer printer;
        // printer.print(std::move(result));
        Interpreter interpreter;
        interpreter.interpret(std::move(result));
  
 
}
catch (const std::runtime_error& e) 
{
        std::cerr << " " << e.what() << std::endl;
        }

    return 0;
}
