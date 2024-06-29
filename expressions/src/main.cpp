
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

       !(true or true)
       
        )";


try
{

//     Lexer lexer = Lexer(code);
//     std::vector<Token> tokens = lexer.scanTokens();
//     for (Token token : tokens)
//     {
//        std::cout << token.toString() << std::endl;
//     }
//     Parser parser = Parser(tokens);
//     std::unique_ptr<Expr> result = parser.result();

//    // Printer printer;
//    // printer.print(std::move(result));
//    Interpreter interpreter;
//    interpreter.interpret(std::move(result));
  // Testes Aritméticos
    Test("5 - 2 + 3", std::make_unique<Literal>(6));
    Test("4 * 2 / 2", std::make_unique<Literal>(4));
    Test("(3 + 2) * 2", std::make_unique<Literal>(10));

    // Testes de Potência
    Test("2 ^ 3", std::make_unique<Literal>(8));
    Test("3 ^ 2 ^ 2", std::make_unique<Literal>(81));
    Test("(2 + 3) ^ 2", std::make_unique<Literal>(25));

    // Testes Lógicos
    Test("true or false", std::make_unique<Literal>(true));
    Test("true and (false or true)", std::make_unique<Literal>(true));
    Test("not (true and false)", std::make_unique<Literal>(true));

    // Testes de Comparação
    Test("5 == 5", std::make_unique<Literal>(true));
    Test("5 != 3", std::make_unique<Literal>(true));
    Test("4 < 2", std::make_unique<Literal>(false));
    Test("3 <= 3", std::make_unique<Literal>(true));
    Test("5 > 2", std::make_unique<Literal>(true));
    Test("4 >= 5", std::make_unique<Literal>(false));

    // Testes de Mistura de Tipos
    Test("1 + 2.5", std::make_unique<Literal>(3.5f));
    Test("2 * 3.0", std::make_unique<Literal>(6.0f));
    Test("true and 1", std::make_unique<Literal>(true));
    Test("false or 0", std::make_unique<Literal>(false));

    // Testes de Expressões Complexas
    Test("1 + 2 * 3 - 4 / 2", std::make_unique<Literal>(5));
    Test("(3 + 5) * (2 - 1)", std::make_unique<Literal>(8));
    Test("2 ^ (1 + 1) * 3", std::make_unique<Literal>(12));
    Test("not true or false and true", std::make_unique<Literal>(false));

     // Testes de igualdade
    Test("5 == 5", std::make_unique<Literal>(true));
    Test("5 != 3", std::make_unique<Literal>(true));
    Test("4 == 2", std::make_unique<Literal>(false));
    Test("3 != 3", std::make_unique<Literal>(false));
    Test("true == true", std::make_unique<Literal>(true));
    Test("false == true", std::make_unique<Literal>(false));
 
}
catch (const std::runtime_error& e) 
{
        std::cerr << " " << e.what() << std::endl;
        }

    return 0;
}
