#include "Windows\Controls\Buttons\Base\HoverButtonWindow.h"

HoverButtonWindow::HoverButtonWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow, function<void()> clickCallback)
    : ChildWindow(context, descriptor, parentWindow), hoverStateDeviceContext(nullptr), normalStateDeviceContext(nullptr)
{
    this->clickCallback = clickCallback;
    this->isHovered = false;
}

void HoverButtonWindow::Initialize()
{
    ChildWindow::Initialize();
    SetWindowLongPtr(windowHandle, GWL_WNDPROC, (LONG_PTR)HoverButtonWindow::WndProc);

    RenderStatesDeviceContext();
    RenderContent(nullptr);
}

Size HoverButtonWindow::RenderContent(Renderer* renderer)
{
    HDC sourceDC = isHovered
        ? hoverStateDeviceContext
        : normalStateDeviceContext;

    context->GetDeviceContextProvider()->CopyDeviceContext(sourceDC, deviceContextBuffer->GetDeviceContext(), windowSize);

    MoveWindow(windowHandle, descriptor.Position.X, descriptor.Position.Y, windowSize.Width, windowSize.Height, FALSE);

    return windowSize;
}

LRESULT CALLBACK HoverButtonWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.hwndTrack = hWnd;
    tme.dwHoverTime = 10;

    HoverButtonWindow* window = (HoverButtonWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    switch (message)
    {

    case WM_LBUTTONUP:
        window->clickCallback();
        return TRUE;

    case WM_MOUSEMOVE:
        tme.dwFlags = TME_HOVER | TME_LEAVE;
        TrackMouseEvent(&tme);

        return TRUE;

    case WM_MOUSEHOVER:
        window->isHovered = true;
        window->RenderContent(nullptr);
        InvalidateRect(hWnd, nullptr, TRUE);

        SetCursor(LoadCursor(nullptr, IDC_HAND));

        break;

    case WM_SETCURSOR:
        return TRUE;

    case WM_MOUSELEAVE:
        window->isHovered = false;
        window->RenderContent(nullptr);
        InvalidateRect(hWnd, nullptr, TRUE);

        SetCursor(LoadCursor(nullptr, IDC_ARROW));

        return TRUE;

    default:
        break;
    }

    return ChildWindow::WndProc(hWnd, message, wParam, lParam);
}

HoverButtonWindow::~HoverButtonWindow()
{
    DeleteDC(normalStateDeviceContext);
    DeleteDC(hoverStateDeviceContext);
}