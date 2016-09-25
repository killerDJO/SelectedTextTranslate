#include "Windows\Base\ChildWindow.h"

ChildWindow::ChildWindow(WindowContext* context, WindowDescriptor descriptor, HWND parentWindow)
    : Window(context, descriptor)
{
    this->parentWindow = parentWindow;

    this->className = L"STT_CONTENT";
}

void ChildWindow::Initialize()
{
    Window::Initialize();
    hWindow = CreateWindow(
        className,
        NULL,
        WS_CHILD | WS_CLIPCHILDREN,
        descriptor.Position.X,
        descriptor.Position.Y,
        descriptor.WindowSize.Width,
        descriptor.WindowSize.Height,
        parentWindow,
        NULL,
        context->GetInstance(),
        this);
}

void ChildWindow::SpecifyWindowClass(WNDCLASSEX* windowClass)
{
    windowClass->lpfnWndProc = WndProc;
}

LRESULT CALLBACK ChildWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    case WM_MOVE:
    {
        RECT rcWindow;
        POINTS pos = MAKEPOINTS(lParam);
        GetWindowRect(hWnd, &rcWindow);
        MoveWindow(hWnd, pos.x, pos.y, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, FALSE);
        InvalidateRect(hWnd, nullptr, FALSE);
        return TRUE;
    }

    default:
        return Window::WndProc(hWnd, message, wParam, lParam);
    }
}

ChildWindow::~ChildWindow()
{
}