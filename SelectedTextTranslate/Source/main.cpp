#include "Application.h"

void AttachConsole()
{
    AllocConsole();
    AttachConsole(GetCurrentProcessId());
    FILE* file;
    freopen_s(&file, "CON", "w", stdout);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR args, int iCmdShow)
{
    // AttachConsole();

    Application application;
    return application.Run(hInstance);
}