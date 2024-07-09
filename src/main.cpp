
#include "pch.h"
#include "Token.hpp"
#include "Literal.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"
#include "Utils.hpp"

#define USE_RAYLIB
//#undef USE_RAYLIB

#ifdef USE_RAYLIB
#include <raylib.h>
#else
int IsMouseButtonDown(int button) { return false; }
int IsMouseButtonPressed(int button) { return false; }
int IsMouseButtonReleased(int button) { return false; }
int IsMouseButtonUp(int button) { return false; }
int GetMouseX() { return 0; }
int GetMouseY() { return 0; }

struct Color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

Color WHITE = {255, 255, 255, 255}; 
Color RED = {255, 0, 0, 255};
Color BLUE = {0, 0, 255, 255};

void DrawCircle(int x, int y, int radius, Color color) {}

void DrawText(const char *text, int x, int y, int fontSize, Color color) {}

#endif

#include <iostream>
#include <fstream>
#include <sstream>

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

LiteralPtr native_mouse_down(ExecutionContext* ctx, int argc) 
{
    if (argc != 1)
    {
        return ctx->asBool(false);
    }
    int button = ctx->getInt(0);

    return ctx->asBool(IsMouseButtonDown(button));
}

LiteralPtr native_mouse_pressed(ExecutionContext* ctx, int argc) 
{
    if (argc != 1)
    {
        return ctx->asBool(false);
    }
    int button = ctx->getInt(0);
    
    return ctx->asBool(IsMouseButtonPressed(button));
}

LiteralPtr native_mouse_x(ExecutionContext* ctx, int argc) 
{
    return ctx->asFloat((float)GetMouseX());
}

LiteralPtr native_mouse_y(ExecutionContext* ctx, int argc) 
{
    return ctx->asFloat((float)GetMouseY());
}

LiteralPtr native_circle(ExecutionContext* ctx, int argc) 
{

    if (argc != 4)
    {
        ctx->Error("Usage: circle(x, y, radius, fill)"+std::to_string(argc));   
        return ctx->asBool(false);
    }
     int x = ctx->getInt(0);
     int y = ctx->getInt(1);
     int radius = ctx->getInt(2);

   
    bool fill = ctx->getBool(3);

    if (fill)
    {
        DrawCircle(x, y, radius, BLUE);
    } else 
    {
       
       DrawCircle(x, y, radius, RED);
    }

    
    return ctx->asBool(true);
}

LiteralPtr native_text(ExecutionContext* ctx, int argc) 
{

    if (argc != 4)
    {
        ctx->Error("Usage: text(x, y, size, string)"+std::to_string(argc));   
        return ctx->asBool(false);
    }
     int x =  ctx->getInt(0);
     int y =  ctx->getInt(1);
     int radius =  ctx->getInt(2);
     std::string text =  ctx->getString(3).c_str();

     DrawText(text.c_str(), x, y, radius, RED);
    
    return ctx->asBool(true);
}



int main()
{
static const NativeFuncDef native_funcs[] = {
    {"writeln", native_writeln},
    {"mouse_down", native_mouse_down},
    {"mouse_pressed", native_mouse_pressed},
    {"mouse_x", native_mouse_x},
    {"mouse_y", native_mouse_y},
    {"circle", native_circle},
    {"text", native_text},
    {NULL, NULL}
};

    try
    {

         std::string code = readFile("main.pc");
         Factory::Instance();
         
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

#ifdef USE_RAYLIB

            const int screenWidth = 800;
            const int screenHeight = 450;

     
            SetTraceLogLevel(LOG_NONE);
            SetTraceLogCallback(Native_TraceLog);

            InitWindow(screenWidth, screenHeight, "BuLang with Raylib");
            SetTargetFPS(60);

            interpreter.build(program);

            

            while (!WindowShouldClose())   
            {

                BeginDrawing();
                ClearBackground(BLACK);
                    if (!interpreter.run())
                    {
                        break;
                    }

                DrawFPS(10,10);
                DrawText(TextFormat("Process: %d", interpreter.Count()), 10, 40, 20, WHITE);
                EndDrawing();
            }
         
            
         
        #else
                interpreter.build(program);
                interpreter.run();
         
        #endif
         
    }
         
   }
 
}
catch (const FatalException& e) 
{
        std::string text = e.what();
        Log(2,"Fatal: %s", text.c_str());
}
catch (const std::runtime_error& e) 
{
        std::string text = e.what();
        Log(2,"Runtime: %s", text.c_str());
}

Factory::Instance().clear();

#ifdef USE_RAYLIB
 CloseWindow(); 
#endif
            

return 0;
}
