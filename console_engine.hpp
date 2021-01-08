#pragma once

#include "geometry_structures.hpp"

#include <windows.h>
#include <winuser.h>

enum event_key
{
    EVENT_W,
    EVENT_S,
    EVENT_A,
    EVENT_D,
    EVENT_UP,
    EVENT_DOWN,
    EVENT_LEFT,
    EVENT_RIGHT,
    EVENT_INVALID
};

class ConsoleEngine
{
public:
    ConsoleEngine()
    {
        input_hnd  = GetStdHandle(STD_INPUT_HANDLE);
        output_hnd = GetStdHandle(STD_OUTPUT_HANDLE);
        window_hnd = GetConsoleWindow();

        InitScreen();
    }

    ~ConsoleEngine()
    {
        delete[] buffer;
    }

    event_key ReadFromConsoleInput();
    void DrawMesh(const std::vector<projected_triangle>* projected_obj);
    void WriteToConsoleOutput();
    void ClearScreenBuffer();

    static void GetScreenSize(uint16_t& width, uint16_t& height)
    {
        width  = screen_width;
        height = screen_height;
    }

private:
    void InitScreen();

    void DrawPixel(const point_2d& point);
    void DrawLine(const point_2d& point_1, const point_2d& point_2);
    void DrawTriangle(const point_2d& point_1, const point_2d& point_2, const point_2d& point_3);
    void DrawBottomFlatTriangle(const point_2d& point_1, const point_2d& point_2, const point_2d& point_3);
    void DrawTopFlatTriangle(const point_2d& point_1, const point_2d& point_2, const point_2d& point_3);

    static uint16_t screen_width;
    static uint16_t screen_height;

    HANDLE input_hnd;
    INPUT_RECORD input_buffer;

    HANDLE output_hnd;
    HWND window_hnd;
    COORD console_size;
    SMALL_RECT window_size;
    CHAR_INFO* buffer;
};
