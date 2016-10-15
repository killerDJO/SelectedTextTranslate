#pragma once
#include "View\Framework\Windows\ChildWindow.h"

class HoverButtonWindow : public ChildWindow
{
private:
    function<void()> clickCallback;

    bool isHovered;

    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

protected:
    HDC hoverStateDeviceContext;
    HDC normalStateDeviceContext;

    Size RenderContent(Renderer* renderer) override;

    virtual void RenderStatesDeviceContext() = 0;

public:
    HoverButtonWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow, function<void()> clickCallback);
    virtual ~HoverButtonWindow();

    void Initialize() override;
};