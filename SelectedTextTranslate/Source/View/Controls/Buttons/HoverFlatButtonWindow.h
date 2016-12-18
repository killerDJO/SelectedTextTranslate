#pragma once
#include "View\Controls\Buttons\Base\HoverButtonWindow.h"

class HoverFlatButtonWindow : public HoverButtonWindow
{
private:
    HFONT defaultFont;
    HFONT font;
    wstring text;

    int paddingX;
    int paddingY;
    int borderWidth;

    void RenderStateDeviceContext(HDC deviceContext, Colors backgroundColor, Colors borderColor, Colors fontColor) const;

protected:
    void RenderStatesDeviceContext() override;

public:
    HoverFlatButtonWindow(WindowContext* context, Window* parentWindow);
    ~HoverFlatButtonWindow();

    void Initialize() override;

    void SetDescriptor(WindowDescriptor descriptor) override;
    void SetPosition(Point position);

    void SetFont(HFONT font);
    HFONT GetFont() const;

    void SetText(wstring text);
    wstring GetText() const;

    void SetPaddingX(int paddingX);
    int GetPaddingX() const;

    void SetPaddingY(int paddingY);
    int GetPaddingY() const;

    Size GetComputedSize() const;

    double GetTextBaseline() const;
};