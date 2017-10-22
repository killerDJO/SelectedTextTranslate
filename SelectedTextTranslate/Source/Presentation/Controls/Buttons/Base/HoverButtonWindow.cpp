#include "Presentation\Controls\Buttons\Base\HoverButtonWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include <set>

HoverButtonWindow::HoverButtonWindow(CommonContext* context, View* parentWindow)
    : ChildView(context, parentWindow)
{
    this->OnClick = Subscribeable<>();
    this->state = ButtonStates::Normal;
    this->stateToDeviceContextMap = map<ButtonStates, HDC>();
}

void HoverButtonWindow::Initialize()
{
    ChildView::Initialize();
    RenderStatesDeviceContexts();
}

Size HoverButtonWindow::RenderContent(Renderer* renderer)
{
    renderer->DrawDeviceContext(stateToDeviceContextMap[state], GetSize());
    return renderer->GetSize();
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
        Render();

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
        Render();
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
            Render();
            SetCursor(LoadCursor(nullptr, IDC_HAND));

            return TRUE;
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
            Render();
            SetCursor(LoadCursor(nullptr, IDC_ARROW));

            return TRUE;
        }

        break;
    }

    }

    return ChildView::WindowProcedure(message, wParam, lParam);
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

    if (viewStateDescriptor.GetViewState() != ViewStates::New)
    {
        Render();
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
            deviceContextProvider->DeleteDeviceContext(iterator->second);
            deletedDeviceContexts.insert(deviceContextToDelete);
        }
    }
}
