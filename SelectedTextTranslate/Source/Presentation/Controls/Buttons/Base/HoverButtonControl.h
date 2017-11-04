#pragma once
#include "Utilities\Subscribable\Subscribable.h"
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
