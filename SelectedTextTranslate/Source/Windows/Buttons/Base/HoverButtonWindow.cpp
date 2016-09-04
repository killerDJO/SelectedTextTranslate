#include "Windows\Buttons\Base\HoverButtonWindow.h"

HoverButtonWindow::HoverButtonWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height, function<void()> clickCallback)
    : ChildWindow(parentWindow, hInstance, x, y, width, height)
{
    this->clickCallback = clickCallback;
    this->isHovered = false;
}

void HoverButtonWindow::Initialize()
{
    ChildWindow::Initialize();
    SetWindowLongPtr(hWindow, GWL_WNDPROC, (LONG_PTR)HoverButtonWindow::WndProc);

    RenderStatesHDC();
    RenderDC();
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
        window->RenderDC();
        InvalidateRect(hWnd, NULL, TRUE);

        SetCursor(LoadCursor(NULL, IDC_HAND));

        break;

    case WM_SETCURSOR:
        return TRUE;

    case WM_MOUSELEAVE:
        window->isHovered = false;
        window->RenderDC();
        InvalidateRect(hWnd, NULL, TRUE);

        SetCursor(LoadCursor(NULL, IDC_ARROW));

        return TRUE;

    default:
        break;
    }

    return ChildWindow::WndProc(hWnd, message, wParam, lParam);
}

POINT HoverButtonWindow::RenderDC()
{
    POINT bottomRight = ChildWindow::RenderDC();

    HDC sourceHDC = isHovered
        ? hoverStateHDC
        : normalStateHDC;

    DWORD res = BitBlt(inMemoryHDC, 0, 0, width, height, sourceHDC, 0, 0, SRCCOPY);

    MoveWindow(hWindow, x, y, width, height, FALSE);

    bottomRight.x = width;
    bottomRight.y = height;

    return bottomRight;
}

HoverButtonWindow::~HoverButtonWindow()
{
    DeleteDC(normalStateHDC);
    DeleteDC(hoverStateHDC);
}