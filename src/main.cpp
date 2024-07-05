
#include "pch.h"
#include "Token.hpp"
#include "Literal.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"
#include "Utils.hpp"


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

         std::string code = readFile("main.pc");
         auto& pool = LiteralPool::Instance();
         pool.setMaxPoolSize(100); // Define o tamanho máximo do pool para 10 objetos


        Lexer lexer = Lexer(code);
        std::vector<Token> tokens = lexer.scanTokens();
         for (Token token : tokens)
         {
             std::cout << token.toString() << std::endl;
         }
         if (lexer.ready())
         {
            Parser parser = Parser(tokens);
            std::shared_ptr<Stmt> program = parser.parse();
            Interpreter interpreter;
            if (program)
            {
            interpreter.execute(program);
            interpreter.run();
            }
         }
        

  
 
}
catch (const FatalException& e) 
{
        std::string text = e.what();
        Log(2,"%s", text.c_str());
}
catch (const std::runtime_error& e) 
{
        std::string text = e.what();
        Log(2,"%s", text.c_str());
}



return 0;
}
