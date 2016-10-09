#pragma once
#include "Windows\Base\ChildWindow.h"
#include "Model\AppModel.h"

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

    AppModel* appModel;

    Size RenderContent(Renderer* renderer) override;

public:
    ContentWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow, AppModel* appModel);
    ~ContentWindow() override;

    void Initialize() override;
};