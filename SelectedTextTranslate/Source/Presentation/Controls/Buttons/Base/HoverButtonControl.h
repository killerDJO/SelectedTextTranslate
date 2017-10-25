#pragma once
#include "Utilities\Subscribeable.h"
#include "Presentation\Controls\Buttons\Base\Enums\ButtonStates.h"
#include "Presentation\Framework\Views\ControlView.h"

class HoverButtonControl : public ControlView
{
private:
    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;
    void ChangeButtonState(ButtonStates newState);

protected:
    ButtonStates state;
    map<ButtonStates, HDC> stateToDeviceContextMap;

    Size RenderContent(Renderer* renderer) override;

    virtual void RenderStatesDeviceContexts() = 0;

public:
    HoverButtonControl(CommonContext* context, View* parentWindow);
    virtual ~HoverButtonControl();

    Subscribeable<> OnClick;

    void Initialize() override;

    void Disable();
    void Enable();
    bool IsDisabled() const;
};
