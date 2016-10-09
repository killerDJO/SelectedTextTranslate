#include "Windows\Base\ChildWindow.h"

ChildWindow::ChildWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow)
    : Window(context, descriptor)
{
    this->parentWindow = parentWindow;

    this->className = L"STT_CONTENT";
}

void ChildWindow::Initialize()
{
    Window::Initialize();
    windowHandle = CreateWindow(
        className,
        NULL,
        WS_CHILD | WS_CLIPCHILDREN,
        descriptor.Position.X,
        descriptor.Position.Y,
        descriptor.WindowSize.Width,
        descriptor.WindowSize.Height,
        parentWindow->GetHandle(),
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
    ChildWindow* instance = (ChildWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    switch (message)
    {

    // This message should be processed in order to enable scrolling to work.
    // Its sent to child windows after ScrollWindow on parent is called.
    case WM_MOVE:
    {
        RECT rcWindow;
        POINTS pos = MAKEPOINTS(lParam);
        GetWindowRect(hWnd, &rcWindow);
        MoveWindow(hWnd, pos.x, pos.y, instance->windowSize.Width, instance->windowSize.Height, FALSE);
        instance->position.X = pos.x;
        instance->position.Y = pos.y;
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