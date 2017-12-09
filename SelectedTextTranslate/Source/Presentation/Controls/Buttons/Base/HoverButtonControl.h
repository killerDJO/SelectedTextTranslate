#pragma once
#include "Utilities\Subscribable\Subscribable.h"
#include "Presentation\Controls\Buttons\Base\Enums\ButtonStates.h"
#include "Presentation\Framework\Views\ControlView.h"

class HoverButtonControl : public ControlView
{
private:
    void ChangeButtonState(ButtonStates newState);

    LRESULT ProcessLeftButtonUp(WPARAM wParam, LPARAM lParam);
    LRESULT ProcessLeftButtonDown(WPARAM wParam, LPARAM lParam);
    LRESULT ProcessMouseMove(WPARAM wParam, LPARAM lParam);
    LRESULT ProcessMouseHover(WPARAM wParam, LPARAM lParam);
    LRESULT ProcessMouseLeave(WPARAM wParam, LPARAM lParam);

protected:
    ButtonStates state;
    map<ButtonStates, HDC> stateToDeviceContextMap;

    void SpecifyWindow(NativeWindowHolder* window) override;
    void RenderContent(Renderer* renderer) override;

    virtual void RenderStatesDeviceContexts() = 0;

public:
    HoverButtonControl(ViewContext* context, View* parentWindow);
    virtual ~HoverButtonControl();

    Subscribable<> OnClick;

    void Initialize() override;

    void Disable();
    void Enable();
    bool IsDisabled() const;
};
