#pragma once
#include "Windows\Base\ChildWindow.h"
#include "Model\AppModel.h"

class ContentWindow : public ChildWindow
{
protected:
    int paddingX;
    int paddingY;
    int lineHeight;

    HFONT fontNormal, fontHeader, fontItalic, fontSmall;
    HBRUSH grayBrush;

    AppModel* appModel;

    Size RenderDC(Renderer* renderer) override;

public:
    ContentWindow(WindowContext* context, WindowDescriptor descriptor, HWND parentWindow, AppModel* appModel);
    ~ContentWindow() override;

    void Initialize() override;
};