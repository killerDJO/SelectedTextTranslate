#pragma once
#include "View\Framework\Windows\ChildWindow.h"

class ContentWindow : public ChildWindow
{
protected:
    int paddingX;
    int paddingY;
    double lineHeight;

    Font* fontNormal;
    Font* fontHeader;
    Font* fontItalic;
    Font* fontSmall;
    Font* fontSmallUnderscored;

    Brush* grayBrush;
    Brush* lightGrayBrush;
    Brush* backgroundBrush;

    Size RenderContent(Renderer* renderer) override;

public:
    ContentWindow(WindowContext* context, Window* parentWindow);
    ~ContentWindow() override;

    double GetLineHeight() const;
    void SetLineHeight(double lineHeight);

    int GetPaddingX() const;
    void SetPaddingX(int paddingX);

    int GetPaddingY() const;
    void SetPaddingY(int paddingY);
};