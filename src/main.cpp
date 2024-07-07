
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

LiteralPtr native_writeln(ExecutionContext* ctx, int argc, LiteralList* argv) 
{
    for (int i = 0; i < argc; i++)
    {
            if (ctx->isString(argv[i]))
            {
                std::cout << argv[i]->getString();
            }
            else if (ctx->isInt(argv[i]))
            {
                std::cout << argv[i]->getInt();
            }
            else if (ctx->isFloat(argv[i]))
            {
                std::cout << argv[i]->getFloat();
            } else 
            {
                std::cout << argv[i]->toString();
            }
    }
    std::cout<< std::endl;
    return ctx->asBool(true);
}


int main()
{
static const NativeFuncDef native_funcs[] = {
    {"writeln", native_writeln},
    {NULL, NULL}
};

    try
    {

         std::string code = readFile("main.pc");
         auto& pool = LiteralPool::Instance();
         pool.setMaxPoolSize(100); // Define o tamanho máximo do pool para 10 objetos


        Lexer lexer = Lexer(code);
        std::vector<Token> tokens = lexer.scanTokens();
         for (Token token : tokens)
         {
             //std::cout << token.toString() << std::endl;
         }
         if (lexer.ready())
         {
            Parser parser = Parser(tokens);
            std::shared_ptr<Stmt> program = parser.parse();
            Interpreter interpreter;
            //interpreter.registerFunction("writeln", native_writeln);
            for (const NativeFuncDef* def = native_funcs; def->name != NULL; def++) 
            {
                interpreter.registerFunction(def->name, def->func);
            }
            if (program)
            {
            
            interpreter.run(std::move(program));
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
