#pragma once
#include "View\Framework\Windows\ChildWindow.h"
#include "Utilities\Subscribeable.h"

class HoverButtonWindow : public ChildWindow
{
private:
    bool isHovered;

    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

protected:
    HDC hoverStateDeviceContext;
    HDC normalStateDeviceContext;

    Size RenderContent(Renderer* renderer) override;

    virtual void RenderStatesDeviceContext() = 0;

public:
    HoverButtonWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow);
    virtual ~HoverButtonWindow();

    Subscribeable<> OnClick;

    void Initialize() override;
};