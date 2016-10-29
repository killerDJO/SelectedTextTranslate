#include "View\Controls\Buttons\Base\HoverButtonWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

HoverButtonWindow::HoverButtonWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow)
    : ChildWindow(context, descriptor, parentWindow)
{
    this->OnClick = Subscribeable<>();
    this->isHovered = false;

    this->hoverStateDeviceContext = nullptr;
    this->normalStateDeviceContext = nullptr;
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

    AssertCriticalWinApiResult(MoveWindow(windowHandle, descriptor.GetPosition().X, descriptor.GetPosition().Y, windowSize.Width, windowSize.Height, FALSE));

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
            OnClick.Notify();
            return TRUE;

        case WM_MOUSEMOVE:
            tme.dwFlags = TME_HOVER | TME_LEAVE;
            AssertCriticalWinApiResult(TrackMouseEvent(&tme));

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
    AssertWinApiResult(DeleteDC(normalStateDeviceContext));
    AssertWinApiResult(DeleteDC(hoverStateDeviceContext));
}