#pragma once
#include "View\Framework\Windows\ChildWindow.h"

class ContentWindow : public ChildWindow
{
protected:
    int paddingX;
    int paddingY;
    int lineHeight;

    Font* fontNormal;
    Font* fontHeader;
    Font* fontItalic;
    Font* fontSmall;
    Font* fontSmallUnderscored;

    Brush* grayBrush;
    Brush* lightGrayBrush;
    Brush* backgroundBrush;

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