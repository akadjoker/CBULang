
#include "pch.h"
#include "Token.hpp"
#include "Literal.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"
#include "Utils.hpp"
#include "raylib.h"

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

LiteralPtr native_writeln(ExecutionContext* ctx, int argc, LiteralList* argv) 
{
    for (int i = 0; i < argc; i++)
    {
            if (ctx->isString(argv[i]))
            {
                std::cout << argv[i]->getString();
            }
            else if (ctx->isNumber(argv[i]))
            {
                std::cout << argv[i]->getNumber();
            } else 
            {
                std::cout << argv[i]->toString();
            }
    }
    std::cout<< std::endl;
    return ctx->asBool(true);
}

LiteralPtr native_mouse_down(ExecutionContext* ctx, int argc, LiteralList* argv) 
{
    if (argc != 1)
    {
        return ctx->asBool(false);
    }
    int button = argv[0]->getInt();
    
    return ctx->asBool(IsMouseButtonDown(button));
}

LiteralPtr native_mouse_pressed(ExecutionContext* ctx, int argc, LiteralList* argv) 
{
    if (argc != 1)
    {
        return ctx->asBool(false);
    }
    int button = argv[0]->getInt();
    
    return ctx->asBool(IsMouseButtonPressed(button));
}

LiteralPtr native_mouse_x(ExecutionContext* ctx, int argc, LiteralList* argv) 
{
    return ctx->asFloat(GetMouseX());
}

LiteralPtr native_mouse_y(ExecutionContext* ctx, int argc, LiteralList* argv) 
{
    return ctx->asFloat(GetMouseY());
}

LiteralPtr native_circle(ExecutionContext* ctx, int argc, LiteralList* argv) 
{

    if (argc != 4)
    {
        ctx->Error("Usage: circle(x, y, radius, fill)"+std::to_string(argc));   
        return ctx->asBool(false);
    }
     int x = argv[0]->getInt();
     int y = argv[1]->getInt();
     int radius = argv[2]->getInt();

   
    bool fill = argv[3]->getBool();

    if (fill)
    {
        DrawCircle(x, y, radius, BLUE);
    } else 
    {
       
       DrawCircle(x, y, radius, RED);
    }

    
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
    {NULL, NULL}
};

    try
    {

         std::string code = readFile("main.pc");
         LiteralPool::Instance();
         
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
         
            
          //  interpreter.run(program);
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

 CloseWindow(); 
            

return 0;
}
