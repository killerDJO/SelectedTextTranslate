#pragma once
#include "Utilities\Subscribeable.h"
#include "Presentation\Controls\Buttons\Base\Enums\ButtonStates.h"
#include "Presentation\Framework\Views\ChildView.h"

class HoverButtonWindow : public ChildView
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
    HoverButtonWindow(CommonContext* context, View* parentWindow);
    virtual ~HoverButtonWindow();

    Subscribeable<> OnClick;

    void Initialize() override;

    void Disable();
    void Enable();
    bool IsDisabled() const;
};
