#pragma once

#include "console_engine.hpp"
#include "projection_engine.hpp"

class Application
{
public:
    Application() : console_engine(), projection_engine()
    {
        
    }

    ~Application()
    {

    }

    void OnElapsedTime(float time_elapsed)
    {

    }

private:

    ConsoleEngine* console_engine;
    ProjectionEngine* projection_engine;
};