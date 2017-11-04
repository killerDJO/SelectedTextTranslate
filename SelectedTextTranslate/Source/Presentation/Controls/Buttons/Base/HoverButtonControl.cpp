#include "Presentation\Controls\Buttons\Base\HoverButtonControl.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

HoverButtonControl::HoverButtonControl(ViewContext* context, View* parentWindow)
    : ControlView(context, parentWindow)
{
    this->OnClick = Subscribable<>();
    this->state = ButtonStates::Normal;
    this->stateToDeviceContextMap = map<ButtonStates, HDC>();
}

void HoverButtonControl::Initialize()
{
    ControlView::Initialize();
    RenderStatesDeviceContexts();
}

void HoverButtonControl::RenderContent(Renderer* renderer)
{
    renderer->DrawDeviceContext(stateToDeviceContextMap[state], GetBoundingRect().GetSize());
}

LRESULT HoverButtonControl::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.hwndTrack = Handle;
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

        OnClick();

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

    return ControlView::WindowProcedure(message, wParam, lParam);
}

void HoverButtonControl::Disable()
{
    ChangeButtonState(ButtonStates::Disabled);
}

void HoverButtonControl::Enable()
{
    ChangeButtonState(ButtonStates::Normal);
}

void HoverButtonControl::ChangeButtonState(ButtonStates newState)
{
    state = newState;

    if (State->GetViewState() != ViewStates::New)
    {
        Render();
    }
}

bool HoverButtonControl::IsDisabled() const
{
    return state == ButtonStates::Disabled;
}

HoverButtonControl::~HoverButtonControl()
{
    set<HDC> deletedDeviceContexts;
    for (auto iterator = stateToDeviceContextMap.begin(); iterator != stateToDeviceContextMap.end(); ++iterator)
    {
        HDC deviceContextToDelete = iterator->second;
        if(deletedDeviceContexts.find(deviceContextToDelete) == deletedDeviceContexts.end())
        {
            DeviceContextProvider->DeleteDeviceContext(iterator->second);
            deletedDeviceContexts.insert(deviceContextToDelete);
        }
    }
}
