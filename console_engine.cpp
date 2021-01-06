#include "console_engine.hpp"
#include "projection_engine.hpp"
#include <math.h>
#include <iostream>
#include <cstring>

uint16_t ConsoleEngine::screen_width = 0;
uint16_t ConsoleEngine::screen_height = 0;

void ConsoleEngine::InitScreen()
{
    ShowWindow(window_hnd, SW_MAXIMIZE);
    CONSOLE_SCREEN_BUFFER_INFO buff_info;
    CONSOLE_FONT_INFOEX font_info = {sizeof(font_info),
                                    0,
                                    5,
                                    5,
                                    FF_DONTCARE,
                                    FW_NORMAL};
    wcscpy(font_info.FaceName, L"Lucida Console");
    SetCurrentConsoleFontEx(output_hnd, FALSE, &font_info);

    if (!GetConsoleScreenBufferInfo(output_hnd, &buff_info))
    {
        std::printf("get consoles screen bufer info failed with %d\n", GetLastError());
    }
    console_size = {buff_info.dwMaximumWindowSize.X,
                    buff_info.dwMaximumWindowSize.Y};

    screen_width = console_size.X;
    screen_height = console_size.Y;

    window_size = {0,
                    0,
                    (SHORT)screen_width - 1,
                    (SHORT)screen_height - 1};

    if (!SetConsoleWindowInfo(output_hnd, true, &window_size))
    {
        DWORD err = GetLastError();
        std::printf("SetConsoleWindowInfo failed with %d \n", err);
    }

    SetConsoleDisplayMode(output_hnd, CONSOLE_FULLSCREEN_MODE, &console_size);

    console_size = {(SHORT)screen_width,
                    (SHORT)screen_height};
    if (!SetConsoleScreenBufferSize(output_hnd, console_size))
    {
        DWORD err = GetLastError();
        std::printf("SetConsoleScreenBufferSize failed with %d \n", err);
    }

    if (!SetConsoleActiveScreenBuffer(output_hnd))
    {
        DWORD err = GetLastError();
        std::printf("SetConsoleActiveScreenBuffer failed with %d \n", err);
    }

    buffer = new CHAR_INFO[screen_width * screen_height];
    std::memset(buffer, 0, sizeof(CHAR_INFO) * screen_width * screen_height);
}

void ConsoleEngine::WriteToConsoleOutput()
{
    if(!WriteConsoleOutput(output_hnd, buffer, console_size, {0, 0}, &window_size) )
    {
        DWORD err = GetLastError();
        std::printf("writeConsoleOutput failed! %d", err);
    }
}

void ConsoleEngine::DrawPixel(const point_2d& point)
{
    buffer[point.x + (point.y * screen_width)].Char.UnicodeChar = 0x2588;
    buffer[point.x + (point.y * screen_width)].Attributes = 0x000F;
}

void ConsoleEngine::DrawLine(const point_2d& point_1, const point_2d& point_2)
{
    int16_t abs_x_diff = abs(point_2.x - point_1.x);
    int16_t abs_y_diff = abs(point_2.y - point_1.y);

    int16_t len = abs_x_diff > abs_y_diff ? abs_x_diff : abs_y_diff;

    float dx = (float)(point_2.x - point_1.x) / (float)len;
    float dy = (float)(point_2.y - point_1.y) / (float)len;

    float x = point_1.x;
    float y = point_1.y;

    for(int16_t i = 0; i < len; i++)
    {
        DrawPixel({ (int16_t)roundf(x),
                    (int16_t)roundf(y) });
        x += dx;
        y += dy;
    }

    DrawPixel(point_2);
}

void ConsoleEngine::DrawTriangle(const point_2d& point_1, const point_2d& point_2, const point_2d& point_3)
{
    DrawLine(point_1, point_2);
    DrawLine(point_2, point_3);
    DrawLine(point_3, point_1);
}

void ConsoleEngine::DrawBottomFlatTriangle(const point_2d& point_1, const point_2d& point_2, const point_2d& point_3)
{

}

void ConsoleEngine::DrawTopFlatTriangle(const point_2d& point_1, const point_2d& point_2, const point_2d& point_3)
{

}