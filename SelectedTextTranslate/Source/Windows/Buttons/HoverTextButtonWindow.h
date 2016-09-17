#pragma once
#include "Windows\Buttons\Base\HoverButtonWindow.h"

class HoverTextButtonWindow : public HoverButtonWindow
{
protected:
    void RenderStatesDC() override;

private:
    HFONT font;
    Colors normalColor;
    Colors hoverColor;
    wstring text;

    void RenderStateDC(HDC hdc, Colors color);

public:
    HoverTextButtonWindow(HINSTANCE hInstance, RenderingContext* renderingContext, ScrollProvider* scrollProvider, WindowDescriptor descriptor, HWND parentWindow, HFONT font, Colors normalColor, Colors hoverColor, wstring text, function<void()> clickCallback);
    ~HoverTextButtonWindow();
};