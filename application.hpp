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

        // STATIC DEFINED TEST 3D MESH
        cube.polygons = {// SOUTH
                        {{ {0.0f, 0.0f, -18.0f},  {0.0f, 10.0f, -18.0f},  {10.0f, 10.0f, -18.0f} }},
                        {{ {0.0f, 0.0f, -18.0f},  {10.0f, 10.0f, -18.0f},  {10.0f, 0.0f, -18.0f} }},
                         // EAST
                        {{ {10.0f, 0.0f, -18.0f},  {10.0f, 10.0f, -18.0f},  {10.0f, 10.0f, -28.0f} }},
                        {{ {10.0f, 0.0f, -18.0f},  {10.0f, 10.0f, -28.0f},  {10.0f, 0.0f, -28.0f} }},
                         // NORTH
                        {{ {10.0f, 0.0f, -28.0f},  {10.0f, 10.0f, -28.0f},  {0.0f, 10.0f, -28.0f} }},
                        {{ {10.0f, 0.0f, -28.0f},  {0.0f, 10.0f, -28.0f},  {0.0f, 0.0f, -28.0f} }},
                         // WEST
                        {{ {0.0f, 0.0f, -28.0f},  {0.0f, 10.0f, -28.0f},  {0.0f, 10.0f, -18.0f} }},
                        {{ {0.0f, 0.0f, -28.0f},  {0.0f, 10.0f, -18.0f},  {0.0f, 0.0f, -18.0f} }},
                         // TOP
                        {{ {0.0f, 10.0f, -18.0f},  {0.0f, 10.0f, -28.0f},  {10.0f, 10.0f, -28.0f} }},
                        {{ {0.0f, 10.0f, -18.0f},  {10.0f, 10.0f, -28.0f},  {10.0f, 10.0f, -18.0f} }},
                         // BOTTOM
                        {{ {10.0f, 0.0f, -28.0f},  {0.0f, 0.0f, -28.0f},  {0.0f, 0.0f, -18.0f} }},
                        {{ {10.0f, 0.0f, -28.0f},  {0.0f, 0.0f, -18.0f},  {10.0f, 0.0f, -18.0f} }} };
    }

    ~Application()
    {
        delete projection_engine;
        delete console_engine;
    }


    void Update()
    {
        event_key e = console_engine->ReadFromConsoleInput();
       
        if(e != EVENT_INVALID)
        {
            projection_engine->TransformObject(&cube, e);
        }

        std::vector<projected_triangle> rendered_mesh;
        projection_engine->RenderObject3D(&cube, &rendered_mesh);


        console_engine->DrawMesh(&rendered_mesh);
        console_engine->WriteToConsoleOutput();
        console_engine->ClearScreenBuffer();
    }

private:

    ConsoleEngine* console_engine;
    ProjectionEngine* projection_engine;

    object_3d cube;
};