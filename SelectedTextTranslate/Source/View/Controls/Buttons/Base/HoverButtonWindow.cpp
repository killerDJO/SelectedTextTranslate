#include "View\Controls\Buttons\Base\HoverButtonWindow.h"

HoverButtonWindow::HoverButtonWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow, function<void()> clickCallback)
    : ChildWindow(context, descriptor, parentWindow), hoverStateDeviceContext(nullptr), normalStateDeviceContext(nullptr)
{
    this->clickCallback = clickCallback;
    this->isHovered = false;
}

void HoverButtonWindow::Initialize()
{
    ChildWindow::Initialize();

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

LRESULT HoverButtonWindow::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.hwndTrack = windowHandle;
    tme.dwHoverTime = 10;

    switch (message)
    {

    case WM_LBUTTONUP:
        clickCallback();
        return TRUE;

    case WM_MOUSEMOVE:
        tme.dwFlags = TME_HOVER | TME_LEAVE;
        TrackMouseEvent(&tme);

        return TRUE;

    case WM_MOUSEHOVER:
    {
        if (!isHovered)
        {
            isHovered = true;
            RenderContent(nullptr);
            InvalidateRect(windowHandle, nullptr, TRUE);

            SetCursor(LoadCursor(nullptr, IDC_HAND));
        }

        break;
    }


    case WM_SETCURSOR:
        return TRUE;

    case WM_MOUSELEAVE:
    {
        if(isHovered)
        {
            isHovered = false;
            RenderContent(nullptr);
            InvalidateRect(windowHandle, nullptr, TRUE);

            SetCursor(LoadCursor(nullptr, IDC_ARROW));

            return TRUE;
        }

        break;
    }

    }

    return ChildWindow::WindowProcedure(message, wParam, lParam);
}

HoverButtonWindow::~HoverButtonWindow()
{
    DeleteDC(normalStateDeviceContext);
    DeleteDC(hoverStateDeviceContext);
}