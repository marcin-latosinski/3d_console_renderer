#include "console_engine.hpp"
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

event_key ConsoleEngine::ReadFromConsoleInput()
{
    DWORD n_read_events = 0;
    ReadConsoleInput(
                input_hnd,
                input_buffer,
                128, 
                &n_read_events);

    event_key e_out;
    while (n_read_events-- > 0)
    {
        INPUT_RECORD* e = &input_buffer[n_read_events];
        if (e->EventType == KEY_EVENT && e->Event.KeyEvent.bKeyDown)
        {
            switch (e->Event.KeyEvent.wVirtualKeyCode)
            {
                case 0x57: //W Key
                    e_out = EVENT_W;
                    break;
                case 0x53: //S Key
                    e_out = EVENT_S;
                    break;
                case 0x41: //A Key
                    e_out = EVENT_A;
                    break;
                case 0x44: //D Key
                    e_out = EVENT_D;
                    break;
                case VK_UP:
                    e_out = EVENT_UP;
                    break;
                case VK_DOWN:
                    e_out = EVENT_DOWN;
                    break;
                case VK_LEFT:
                    e_out = EVENT_LEFT;
                    break;
                case VK_RIGHT:
                    e_out = EVENT_RIGHT;
                    break;
                default:
                    e_out = EVENT_INVALID;
                    break;
            }
        }
    }
    return e_out;
}

void ConsoleEngine::WriteToConsoleOutput()
{
    if(!WriteConsoleOutput(output_hnd, buffer, console_size, {0, 0}, &window_size) )
    {
        DWORD err = GetLastError();
        std::printf("writeConsoleOutput failed! %d", err);
    }
}

void ConsoleEngine::ClearScreenBuffer()
{
    std::memset(buffer, 0, sizeof(CHAR_INFO) * screen_width * screen_height);
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

void ConsoleEngine::DrawMesh(const std::vector<projected_triangle>* mesh)
{
    for(int i = 0; i < mesh->size(); i++)
    {
        const projected_triangle& face = mesh->at(i);
        point_2d p1 = { face.points[0].x, face.points[0].y };
        point_2d p2 = { face.points[1].x, face.points[1].y };
        point_2d p3 = { face.points[2].x, face.points[2].y };
        DrawTriangle(p1, p2, p3);
    }
}

void ConsoleEngine::DrawBottomFlatTriangle(const point_2d& point_1, const point_2d& point_2, const point_2d& point_3)
{

}

void ConsoleEngine::DrawTopFlatTriangle(const point_2d& point_1, const point_2d& point_2, const point_2d& point_3)
{

}

