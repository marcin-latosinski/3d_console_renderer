#pragma once

#include "geometry_structures.hpp"

#include <windows.h>

class ConsoleEngine
{
public:
    ConsoleEngine()
    {
        output_hnd = GetStdHandle(STD_OUTPUT_HANDLE);
        window_hnd = GetConsoleWindow();

        // STATIC DEFINED TEST 3D MESH
        cube.polygons = {// SOUTH
                         {0.0f, 0.0f, 0.0f},  {0.0f, 1.0f, 0.0f},  {1.0f, 1.0f, 0.0f},
                         {0.0f, 0.0f, 0.0f},  {1.0f, 1.0f, 0.0f},  {1.0f, 0.0f, 0.0f},
                         // EAST                                                      
                         {1.0f, 0.0f, 0.0f},  {1.0f, 1.0f, 0.0f},  {1.0f, 1.0f, 1.0f},
                         {1.0f, 0.0f, 0.0f},  {1.0f, 1.0f, 1.0f},  {1.0f, 0.0f, 1.0f},
                         // NORTH                                                     
                         {1.0f, 0.0f, 1.0f},  {1.0f, 1.0f, 1.0f},  {0.0f, 1.0f, 1.0f},
                         {1.0f, 0.0f, 1.0f},  {0.0f, 1.0f, 1.0f},  {0.0f, 0.0f, 1.0f},
                         // WEST                                                      
                         {0.0f, 0.0f, 1.0f},  {0.0f, 1.0f, 1.0f},  {0.0f, 1.0f, 0.0f},
                         {0.0f, 0.0f, 1.0f},  {0.0f, 1.0f, 0.0f},  {0.0f, 0.0f, 0.0f},
                         // TOP                                                       
                         {0.0f, 1.0f, 0.0f},  {0.0f, 1.0f, 1.0f},  {1.0f, 1.0f, 1.0f},
                         {0.0f, 1.0f, 0.0f},  {1.0f, 1.0f, 1.0f},  {1.0f, 1.0f, 0.0f},
                         // BOTTOM                                                    
                         {1.0f, 0.0f, 1.0f},  {0.0f, 0.0f, 1.0f},  {0.0f, 0.0f, 0.0f},
                         {1.0f, 0.0f, 1.0f},  {0.0f, 0.0f, 0.0f},  {1.0f, 0.0f, 0.0f} };
    }

    ~ConsoleEngine()
    {
        delete[] buffer;
    }

    void InitScreen();
    void WriteToConsoleOutput();
    void DrawPixel(const point_2d& point);
    void DrawLine(const point_2d& point_1, const point_2d& point_2);
    void DrawTriangle(const point_2d& point_1, const point_2d& point_2, const point_2d& point_3);

    static void GetScreenSize(uint16_t& width, uint16_t& height)
    {
        width  = screen_width;
        height = screen_height;
    }

    static uint16_t screen_width;
    static uint16_t screen_height;//MOVE TO PRIVATE LATER
private:

    void DrawBottomFlatTriangle(const point_2d& point_1, const point_2d& point_2, const point_2d& point_3);
    void DrawTopFlatTriangle(const point_2d& point_1, const point_2d& point_2, const point_2d& point_3);

    void CreateTestMesh();

    HANDLE output_hnd;
    HWND window_hnd;
    COORD console_size;
    SMALL_RECT window_size;
    CHAR_INFO* buffer;

    object_3d cube;
};