#pragma once

#include "console_engine.hpp"
#include "projection_engine.hpp"

class Application
{
public:
    Application() : console_engine(), projection_engine()
    {
        // ConsoleEngine must be created before ProjectionEngine
        console_engine    = new ConsoleEngine();
        projection_engine = new ProjectionEngine();
    }

    ~Application()
    {
        delete projection_engine;
        delete console_engine;
    }

    void DrawCube()
    {
        object_3d* test_mesh = projection_engine->GetCubePtr();
        projection_engine->RenderObject3D(test_mesh);
        std::vector<projected_triangle>* rendered_mesh = projection_engine->GetProjectedMeshPtr();

        console_engine->DrawMesh(rendered_mesh);

        console_engine->WriteToConsoleOutput();
    }

    void OnElapsedTime(float time_elapsed)
    {

    }

private:

    ConsoleEngine* console_engine;
    ProjectionEngine* projection_engine;
};