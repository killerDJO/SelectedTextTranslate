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
    HoverFlatButtonWindow(WindowContext* context, Window* parentWindow);
    ~HoverFlatButtonWindow();

    void SetDescriptor(WindowDescriptor descriptor) override;
    void SetDimensions(Point position, Size size);

    void SetFont(HFONT font);
    HFONT GetFont() const;
    void SetText(wstring text);
    wstring GetText() const;
    void SetPadding(int padding);
    int GetPadding() const;
};