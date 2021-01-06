#include "console_engine.hpp"
#include <windows.h>
#include <cstring>

int main()
{
    ConsoleEngine* engine = new ConsoleEngine();
    engine->InitScreen();

    engine->DrawLine({3, 3}, {9, 8});
    engine->DrawLine({10, 10}, {5, 5});

    engine->DrawTriangle({15,15},
                         {25,15},
                         {20,20});

    engine->DrawPixel({0, 0});
    engine->DrawPixel({0, engine->screen_height - 1});
    engine->DrawPixel({engine->screen_width - 1, 0});
    engine->DrawPixel({engine->screen_width - 1, engine->screen_height - 1});

    engine->WriteToConsoleOutput();

    while(1)
    {

    }
    return 0;
}
