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

void HoverButtonControl::SpecifyWindow(NativeWindowHolder* window)
{
    window
        ->SetMessageHandler(WM_LBUTTONDOWN, bind(&HoverButtonControl::ProcessLeftButtonDown, this, _1, _2))
        ->SetMessageHandler(WM_LBUTTONUP, bind(&HoverButtonControl::ProcessLeftButtonUp, this, _1, _2))
        ->SetMessageHandler(WM_MOUSEMOVE, bind(&HoverButtonControl::ProcessMouseMove, this, _1, _2))
        ->SetMessageHandler(WM_MOUSEHOVER, bind(&HoverButtonControl::ProcessMouseHover, this, _1, _2))
        ->SetMessageHandler(WM_MOUSELEAVE, bind(&HoverButtonControl::ProcessMouseLeave, this, _1, _2))
        ->SetMessageHandler(WM_SETCURSOR, TRUE);
}

void HoverButtonControl::RenderContent(Renderer* renderer)
{
    renderer->DrawDeviceContext(stateToDeviceContextMap[state], GetBoundingRect().GetSize());
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

    if (ViewState->GetViewStatus() != ViewStatus::New)
    {
        Render();
    }
}

bool HoverButtonControl::IsDisabled() const
{
    return state == ButtonStates::Disabled;
}

LRESULT HoverButtonControl::ProcessLeftButtonUp(WPARAM wParam, LPARAM lParam)
{
    if (state == ButtonStates::Disabled)
    {
        return TRUE;
    }

    state = ButtonStates::Hovered;
    Render();

    OnClick();

    return TRUE;
}

LRESULT HoverButtonControl::ProcessLeftButtonDown(WPARAM wParam, LPARAM lParam)
{
    if (state == ButtonStates::Disabled)
    {
        return TRUE;
    }

    state = ButtonStates::Pressed;
    Render();
    return TRUE;
}

LRESULT HoverButtonControl::ProcessMouseMove(WPARAM wParam, LPARAM lParam)
{
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.hwndTrack = Window->GetHandle();
    tme.dwHoverTime = 10;
    tme.dwFlags = TME_HOVER | TME_LEAVE;
    AssertCriticalWinApiResult(TrackMouseEvent(&tme));

    return TRUE;
}

LRESULT HoverButtonControl::ProcessMouseHover(WPARAM wParam, LPARAM lParam)
{
    if (state == ButtonStates::Normal)
    {
        state = ButtonStates::Hovered;
        Render();
        SetCursor(LoadCursor(nullptr, IDC_HAND));
    }

    return TRUE;
}

LRESULT HoverButtonControl::ProcessMouseLeave(WPARAM wParam, LPARAM lParam)
{
    if (state != ButtonStates::Normal && state != ButtonStates::Disabled)
    {
        state = ButtonStates::Normal;
        Render();
        SetCursor(LoadCursor(nullptr, IDC_ARROW));
    }

    return TRUE;
}

HoverButtonControl::~HoverButtonControl()
{
    set<HDC> deletedDeviceContexts;
    for (auto iterator = stateToDeviceContextMap.begin(); iterator != stateToDeviceContextMap.end(); ++iterator)
    {
        HDC deviceContextToDelete = iterator->second;
        if (deletedDeviceContexts.find(deviceContextToDelete) == deletedDeviceContexts.end())
        {
            DeviceContextProvider->DeleteDeviceContext(iterator->second);
            deletedDeviceContexts.insert(deviceContextToDelete);
        }
    }
    deletedDeviceContexts.clear();
}