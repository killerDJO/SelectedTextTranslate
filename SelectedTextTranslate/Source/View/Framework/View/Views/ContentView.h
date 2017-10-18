#pragma once
#include "View\Framework\View\Views\ChildView.h"

class ContentView : public ChildView
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
    ContentView(ViewContext* context, View* parentView);
    ~ContentView() override;

    void Render(bool preserveScrolls = false) override;

    int GetLineHeight() const;
    void SetLineHeight(int lineHeight);

    int GetPaddingX() const;
    void SetPaddingX(int paddingX);

    int GetPaddingY() const;
    void SetPaddingY(int paddingY);
};