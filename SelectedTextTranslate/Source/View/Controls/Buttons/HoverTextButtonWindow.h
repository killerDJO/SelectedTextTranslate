#pragma once
#include "View\Controls\Buttons\Base\HoverButtonWindow.h"

class HoverTextButtonWindow : public HoverButtonWindow
{
private:
    Font* defaultFont;
    Font* font;
    Colors normalColor;
    Colors hoverColor;
    Colors disabledColor;
    Colors backgroundColor;
    wstring text;

    void RenderStateDeviceContext(HDC deviceContext, Colors color) const;

protected:
    void RenderStatesDeviceContexts() override;

public:
    HoverTextButtonWindow(CommonContext* context, View* parentWindow);
    ~HoverTextButtonWindow();

    void SetDescriptor(WindowDescriptor descriptor) override;
    void SetPosition(Point position);

    void SetNormalColor(Colors normalColor);
    Colors GetNormalColor() const;

    void SetHoverColor(Colors hoverColor);
    Colors GetHoverColor() const;

    void SetDisabledColor(Colors disabledColor);
    Colors GetDisabledColor() const;

    void SetBackgroundColor(Colors backgroundColor);
    Colors GetBackgroundColor() const;

    void SetFont(Font* font);
    Font* GetFont() const;

    void SetText(wstring text);
    wstring GetText() const;
};