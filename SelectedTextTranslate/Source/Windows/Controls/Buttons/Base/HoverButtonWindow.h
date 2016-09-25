#pragma once
#include "Windows\Base\ChildWindow.h"

class HoverButtonWindow : public ChildWindow
{
private:
    function<void()> clickCallback;

    bool isHovered;

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
    HDC hoverStateDC;
    HDC normalStateDC;

    Size RenderDC(Renderer* renderer) override;
    virtual void RenderStatesDC() = 0;

public:
    HoverButtonWindow(WindowContext* context, WindowDescriptor descriptor, HWND parentWindow, function<void()> clickCallback);
    virtual ~HoverButtonWindow();

    void Initialize() override;
};