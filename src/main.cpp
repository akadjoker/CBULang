
#include "pch.h"
#include "Token.hpp"
#include "Literal.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"



std::string readFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file)
    {
        throw std::runtime_error("Could not open file.");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}


int main()
{

    // std::string code = R"(

    //     program main;

 

    //     begin
           
    //        int a = 10;

    //         print(a);

    //         begin
    //             int c;

    //             print(c);
    //             c =200;
    //             print(c);

    //             c = d;
    //             print(a);

    //         end
        
           
    //     end.
       
       
    //     )";

    try
    {

         std::string code = readFile("main.pas");

        Lexer lexer = Lexer(code);
        std::vector<Token> tokens = lexer.scanTokens();
         for (Token token : tokens)
         {
             std::cout << token.toString() << std::endl;
         }
        Parser parser = Parser(tokens);
        //std::unique_ptr<Expr> result = parser.result();
        std::unique_ptr<Stmt> program = parser.parse();

        Interpreter interpreter;
        if (program)
        {
           interpreter.execute(program);
        }


     

      //  interpreter.interpret(std::move(result));
  
 
}
catch (const std::runtime_error& e) 
{
        std::cerr << " " << e.what() << std::endl;
        }


std::cout<<"End"<<std::endl;
    return 0;
}
