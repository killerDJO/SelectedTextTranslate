#pragma once
#include "View\Controls\Buttons\Base\HoverButtonWindow.h"

class HoverTextButtonWindow : public HoverButtonWindow
{
private:
    HFONT font;
    Colors normalColor;
    Colors hoverColor;
    Colors disabledColor;
    wstring text;

    void RenderStateDeviceContext(HDC deviceContext, Colors color) const;

protected:
    void RenderStatesDeviceContext() override;

public:
    HoverTextButtonWindow(WindowContext* context, Window* parentWindow);
    ~HoverTextButtonWindow();

    void SetDescriptor(WindowDescriptor descriptor) override;
    void SetPosition(Point position);

    void SetNormalColor(Colors normalColor);
    Colors GetNormalColor() const;

    void SetHoverColor(Colors hoverColor);
    Colors GetHoverColor() const;

    void SetDisabledColor(Colors disabledColor);
    Colors GetDisabledColor() const;

    void SetFont(HFONT font);
    HFONT GetFont() const;

    void SetText(wstring text);
    wstring GetText() const;
};