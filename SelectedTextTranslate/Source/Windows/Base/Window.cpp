#include "Windows\Base\Window.h"

Window::Window(HINSTANCE hInstance, Renderer* renderer)
{
    this->hInstance = hInstance;
    this->renderer = renderer;
}

void Window::Initialize()
{
    WNDCLASSEX windowClass = { 0 };

    if (!GetClassInfoEx(hInstance, className, &windowClass))
    {
        windowClass.hInstance = hInstance;
        windowClass.lpszClassName = className;
        windowClass.cbSize = sizeof(WNDCLASSEX);

        SpecifyWindowClass(&windowClass);

        if (!RegisterClassEx(&windowClass))
        {
            FatalAppExit(0, TEXT("Couldn't register window class!"));
        }
    }
}

HWND Window::GetHandle()
{
    return hWindow;
}

HINSTANCE Window::GetInstance()
{
    return hInstance;
}

DWORD Window::GetWidth()
{
    return width;
}

DWORD Window::GetHeight()
{
    return height;
}

void Window::Show()
{
    ShowWindow(hWindow, SW_SHOW);
}

void Window::Hide()
{
    ShowWindow(hWindow, SW_HIDE);
}

Window::~Window()
{
}