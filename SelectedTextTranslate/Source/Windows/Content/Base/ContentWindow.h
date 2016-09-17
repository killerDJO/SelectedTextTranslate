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

    SIZE RenderDC(Renderer* renderer) override;

public:
    ContentWindow(HINSTANCE hInstance, RenderingContext* renderingContext, ScrollProvider* scrollProvider, WindowDescriptor descriptor, HWND parentWindow, AppModel* appModel);
    virtual ~ContentWindow();

    void Initialize() override;
};