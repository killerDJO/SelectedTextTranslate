#include "Windows\Controls\Buttons\Base\HoverButtonWindow.h"

HoverButtonWindow::HoverButtonWindow(HINSTANCE hInstance, RenderingContext* renderingContext, ScrollProvider* scrollProvider, WindowDescriptor descriptor, HWND parentWindow, function<void()> clickCallback)
    : ChildWindow(hInstance, renderingContext, scrollProvider, descriptor, parentWindow), hoverStateDC(nullptr), normalStateDC(nullptr)
{
    this->clickCallback = clickCallback;
    this->isHovered = false;
}

void HoverButtonWindow::Initialize()
{
    ChildWindow::Initialize();
    SetWindowLongPtr(hWindow, GWL_WNDPROC, (LONG_PTR)HoverButtonWindow::WndProc);

    RenderStatesDC();
    RenderDC(nullptr);
}

Size HoverButtonWindow::RenderDC(Renderer* renderer)
{
    HDC sourceDC = isHovered
        ? hoverStateDC
        : normalStateDC;

    renderingContext->CopyDC(sourceDC, inMemoryDC, windowSize);

    MoveWindow(hWindow, descriptor.Position.X, descriptor.Position.Y, windowSize.Width, windowSize.Height, FALSE);

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
        window->RenderDC(nullptr);
        InvalidateRect(hWnd, nullptr, TRUE);

        SetCursor(LoadCursor(nullptr, IDC_HAND));

        break;

    case WM_SETCURSOR:
        return TRUE;

    case WM_MOUSELEAVE:
        window->isHovered = false;
        window->RenderDC(nullptr);
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
    DeleteDC(normalStateDC);
    DeleteDC(hoverStateDC);
}