
#include "pch.h"
#include "Token.hpp"
#include "Literal.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"
#include "Utils.hpp"

#include <raylib.h>


#include <iostream>
#include <fstream>
#include <sstream>


#include "Core.hpp"
extern void register_core(Interpreter *interpreter);


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



void Native_TraceLog(int logLevel, const char *text, va_list args)
{

}

LiteralPtr native_writeln(ExecutionContext* ctx, int argc) 
{
    // for (int i = 0; i < argc; i++)
    // {
    //         if (ctx->isString(argv[i]))
    //         {
    //             std::cout << argv[i].getString();
    //         }
    //         else if (ctx->isInt(argv[i]))
    //         {
    //             std::cout << argv[i].getInt();}
    //         else if (ctx->isFloat(argv[i]))
    //         {
    //             std::cout << argv[i].getFloat();
    //         } else if (ctx->isBool(argv[i]))
    //         {
    //             std::cout << argv[i].getBool();
    //         } else if (ctx->isByte(argv[i]))
    //         {
    //             std::cout << argv[i].getByte();
    //         } 
    //         else 
    //         {
    //             std::cout << argv[i].toString();
    //         }
    // }
    std::cout<< std::endl;
    return ctx->asBool(true);
}





int main()
{

   
         std::string code = readFile("main.pc");
         
         Interpreter interpreter;

         

            const int screenWidth = 800;
            const int screenHeight = 450;

     
            SetTraceLogLevel(LOG_NONE);
            SetTraceLogCallback(Native_TraceLog);

            InitWindow(screenWidth, screenHeight, "BuLang with Raylib");
            SetTargetFPS(1000);


            interpreter.init();

            register_core(&interpreter);

        bool sucess = false;
        std::string text = "";

    try
    {
            if (interpreter.compile(code))
            {
                sucess = true;
            }
        

            while (!WindowShouldClose() && !sucess)   
            {

                

                        if (!interpreter.run())
                        {
                            break;
                        }
                        
                    

                BeginDrawing();

                Scene::Get().UpdateAndRefresh();
       

                EndDrawing();
            }

       
            
 }
catch (const FatalException& e) 
{
        text = e.what();
        Log(2,"Fatal: %s", text.c_str());
}
catch (const std::runtime_error& e) 
{
        text = e.what();
        Log(2,"Runtime: %s", text.c_str());
}         
    
         
       
   
 


interpreter.cleanup();
Scene::Get().Clear();
CloseWindow(); 
Factory::Instance().clear();


 

            

return 0;
}
