#include "View\Controls\Buttons\Base\HoverButtonWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include <set>

HoverButtonWindow::HoverButtonWindow(WindowContext* context, Window* parentWindow)
    : ChildWindow(context, parentWindow)
{
    this->OnClick = Subscribeable<>();
    this->state = ButtonStates::Normal;
    this->stateToDeviceContextMap = map<ButtonStates, HDC>();
}

void HoverButtonWindow::Initialize()
{
    ChildWindow::Initialize();

    RenderStatesDeviceContext();
    RenderContent(nullptr);
}

Size HoverButtonWindow::RenderContent(Renderer* renderer)
{
    HDC sourceDC = stateToDeviceContextMap[state];

    context->GetDeviceContextProvider()->CopyDeviceContext(sourceDC, deviceContextBuffer->GetDeviceContext(), windowSize);

    AssertCriticalWinApiResult(MoveWindow(
        windowHandle,
        descriptor.GetPosition().GetX(),
        descriptor.GetPosition().GetY(),
        windowSize.GetWidth(),
        windowSize.GetHeight(),
        FALSE));

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
    {
        if(state == ButtonStates::Disabled)
        {
            return TRUE;
        }

        state = ButtonStates::Hovered;
        RenderContent(nullptr);
        InvalidateRect(windowHandle, nullptr, TRUE);

        OnClick.Notify();

        return TRUE;
    }

    case WM_LBUTTONDOWN:
    {
        if (state == ButtonStates::Disabled)
        {
            return TRUE;
        }

        state = ButtonStates::Pressed;
        RenderContent(nullptr);
        InvalidateRect(windowHandle, nullptr, TRUE);
        return TRUE;
    }

    case WM_MOUSEMOVE:
    {
        tme.dwFlags = TME_HOVER | TME_LEAVE;
        AssertCriticalWinApiResult(TrackMouseEvent(&tme));

        return TRUE;
    }

    case WM_MOUSEHOVER:
    {
        if (state == ButtonStates::Normal)
        {
            state = ButtonStates::Hovered;
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
        if (state != ButtonStates::Normal && state != ButtonStates::Disabled)
        {
            state = ButtonStates::Normal;
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

void HoverButtonWindow::Disable()
{
    ChangeButtonState(ButtonStates::Disabled);
}

void HoverButtonWindow::Enable()
{
    ChangeButtonState(ButtonStates::Normal);
}

void HoverButtonWindow::ChangeButtonState(ButtonStates newState)
{
    state = newState;

    if (windowState != WindowStates::New)
    {
        RenderContent(nullptr);
        Draw();
    }
}

bool HoverButtonWindow::IsDisabled() const
{
    return state == ButtonStates::Disabled;
}

HoverButtonWindow::~HoverButtonWindow()
{
    set<HDC> deletedDeviceContexts;
    for (auto iterator = stateToDeviceContextMap.begin(); iterator != stateToDeviceContextMap.end(); ++iterator)
    {
        HDC deviceContextToDelete = iterator->second;
        if(deletedDeviceContexts.find(deviceContextToDelete) == deletedDeviceContexts.end())
        {
            AssertCriticalWinApiResult(DeleteDC(iterator->second));
            deletedDeviceContexts.insert(deviceContextToDelete);
        }
    }
}
