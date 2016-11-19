#pragma once
#include "View\Controls\Buttons\Base\HoverButtonWindow.h"

class HoverFlatButtonWindow : public HoverButtonWindow
{
private:
    HFONT font;
    wstring text;
    int padding;

    void RenderStateDeviceContext(HDC deviceContext, Colors backgroundColor, Colors borderColor) const;

protected:
    void RenderStatesDeviceContext() override;

public:
    HoverFlatButtonWindow(
        WindowContext* context,
        WindowDescriptor descriptor,
        wstring name,
        Window* parentWindow,
        HFONT font,
        wstring text,
        int padding = 2);
    ~HoverFlatButtonWindow();
};