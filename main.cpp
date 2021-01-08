#include "application.hpp"
#include <windows.h>
#include <cstring>

int main()
{
    Application app;
    
    while(1)
    {
        app.Update();
    }

    return 0;
}
