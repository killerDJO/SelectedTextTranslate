#pragma once
#include "View\Framework\Windows\ChildWindow.h"
#include "Controllers\AppController.h"

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

    AppController* appController;

    Size RenderContent(Renderer* renderer) override;

public:
    ContentWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow, AppController* appController);
    ~ContentWindow() override;

    void Initialize() override;
};