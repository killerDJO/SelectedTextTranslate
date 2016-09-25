#pragma once
#include "Windows\Controls\Buttons\Base\HoverButtonWindow.h"

class HoverTextButtonWindow : public HoverButtonWindow
{
protected:
    void RenderStatesDC() override;

private:
    HFONT font;
    Colors normalColor;
    Colors hoverColor;
    wstring text;

    void RenderStateDC(HDC hdc, Colors color) const;

public:
    HoverTextButtonWindow(WindowContext* context, WindowDescriptor descriptor, HWND parentWindow, HFONT font, Colors normalColor, Colors hoverColor, wstring text, function<void()> clickCallback);
    ~HoverTextButtonWindow();
};