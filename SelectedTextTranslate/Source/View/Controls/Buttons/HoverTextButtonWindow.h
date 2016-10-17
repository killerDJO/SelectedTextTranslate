#pragma once
#include "View\Controls\Buttons\Base\HoverButtonWindow.h"

class HoverTextButtonWindow : public HoverButtonWindow
{
private:
    HFONT font;
    Colors normalColor;
    Colors hoverColor;
    wstring text;

    void RenderStateDeviceContext(HDC deviceContext, Colors color) const;

protected:
    void RenderStatesDeviceContext() override;

public:
    HoverTextButtonWindow(
        WindowContext* context,
        WindowDescriptor descriptor,
        Window* parentWindow,
        HFONT font,
        Colors normalColor,
        Colors hoverColor,
        wstring text);
    ~HoverTextButtonWindow();
};