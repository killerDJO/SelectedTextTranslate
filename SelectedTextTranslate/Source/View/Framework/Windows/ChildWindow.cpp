#include "View\Framework\Windows\ChildWindow.h"
#include "Helpers\ExceptionHelper.h"

ChildWindow::ChildWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow)
    : Window(context, descriptor)
{
    this->parentWindow = parentWindow;

    this->className = L"STT_CONTENT";
}

void ChildWindow::Initialize()
{
    Window::Initialize();

    Point offset = GetInitialWindowOffset();
    windowHandle = CreateWindow(
        className,
        NULL,
        WS_CHILD | WS_CLIPCHILDREN,
        descriptor.Position.X - offset.X,
        descriptor.Position.Y - offset.Y,
        descriptor.WindowSize.Width,
        descriptor.WindowSize.Height,
        parentWindow->GetHandle(),
        NULL,
        context->GetInstance(),
        this);

    AssertCriticalWinApiResult(windowHandle);
}

Point ChildWindow::GetInitialWindowOffset()
{
    ScrollProvider* scrollProvider = context->GetScrollProvider();
    int offsetY = scrollProvider->GetCurrentScrollOffset(parentWindow, ScrollBars::Vertical);
    int offsetX = scrollProvider->GetCurrentScrollOffset(parentWindow, ScrollBars::Horizontal);

    return Point(offsetX, offsetY);
}

LRESULT ChildWindow::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    // This message should be processed in order to enable scrolling to work.
    // Its sent to child windows after ScrollWindow on parent is called.
    case WM_MOVE:
    {
        RECT rcWindow;
        POINTS pos = MAKEPOINTS(lParam);
        AssertCriticalWinApiResult(GetWindowRect(windowHandle, &rcWindow));
        AssertCriticalWinApiResult(MoveWindow(windowHandle, pos.x, pos.y, windowSize.Width, windowSize.Height, FALSE));
        position.X = pos.x;
        position.Y = pos.y;

        return TRUE;
    }

    default:
        return Window::WindowProcedure(message, wParam, lParam);
    }
}

ChildWindow::~ChildWindow()
{
}