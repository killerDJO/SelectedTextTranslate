#pragma once
#include "View\Framework\Windows\ChildWindow.h"
#include "Utilities\Subscribeable.h"
#include "View\Controls\Buttons\Base\Enums\ButtonStates.h"

class HoverButtonWindow : public ChildWindow
{
private:
    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

protected:
    ButtonStates state;
    map<ButtonStates, HDC> stateToDeviceContextMap;

    Size RenderContent(Renderer* renderer) override;

    virtual void RenderStatesDeviceContext() = 0;

public:
    HoverButtonWindow(WindowContext* context, WindowDescriptor descriptor, wstring name, Window* parentWindow);
    virtual ~HoverButtonWindow();

    Subscribeable<> OnClick;

    void Initialize() override;
};
