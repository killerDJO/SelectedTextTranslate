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

    HBRUSH grayBrush;

    Size RenderContent(Renderer* renderer) override;

public:
    ContentWindow(WindowContext* context, WindowDescriptor descriptor, wstring name, Window* parentWindow);
    ~ContentWindow() override;

    void Initialize() override;
};