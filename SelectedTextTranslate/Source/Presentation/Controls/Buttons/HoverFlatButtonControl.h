#pragma once
#include "Presentation\Controls\Buttons\Base\HoverButtonControl.h"

class HoverFlatButtonControl : public HoverButtonControl
{
private:
    Font* defaultFont;
    Font* font;
    wstring text;

    int paddingX;
    int paddingY;
    int borderWidth;

    void RenderStateDeviceContext(HDC deviceContext, Colors backgroundColor, Colors borderColor, Colors fontColor) const;

protected:
    void RenderStatesDeviceContexts() override;

public:
    HoverFlatButtonControl(CommonContext* context, View* parentWindow);
    ~HoverFlatButtonControl();

    void Initialize() override;

    void SetPosition(Point position);

    void SetFont(Font* font);
    Font* GetFont() const;

    void SetText(wstring text);
    wstring GetText() const;

    void SetPaddingX(int paddingX);
    int GetPaddingX() const;

    void SetPaddingY(int paddingY);
    int GetPaddingY() const;

    Size GetComputedSize() const;

    int GetTextBaseline() const;
};