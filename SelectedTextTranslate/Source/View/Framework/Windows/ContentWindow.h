#pragma once
#include "View\Framework\Windows\ChildWindow.h"

class ContentWindow : public ChildWindow
{
protected:
    int paddingX;
    int paddingY;
    int lineHeight;

    HFONT fontNormal;
    HFONT fontHeader;
    HFONT fontItalic;
    HFONT fontSmall;
    HFONT fontSmallUnderscored;

    HBRUSH grayBrush;
    HBRUSH lightGrayBrush;
    HBRUSH backgroundBrush;

    Size RenderContent(Renderer* renderer) override;

public:
    ContentWindow(WindowContext* context, Window* parentWindow);
    ~ContentWindow() override;

    int GetLineHeight() const;
    void SetLineHeight(int lineHeight);

    int GetPaddingX() const;
    void SetPaddingX(int paddingX);

    int GetPaddingY() const;
    void SetPaddingY(int paddingY);
};