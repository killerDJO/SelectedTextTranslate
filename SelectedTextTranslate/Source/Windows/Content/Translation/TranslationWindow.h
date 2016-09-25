#pragma once
#include "Windows\Content\Base\ContentWindow.h"

class TranslationWindow : public ContentWindow
{
private:
    const int headerHeight = 50;
    HBRUSH separatorBrush;

    void RenderSeparator(Renderer* renderer, int width);

protected:
    Size RenderDC(Renderer* renderer) override;

public:
    TranslationWindow(WindowContext* context, WindowDescriptor descriptor, HWND parentWindow, AppModel* appModel);
    ~TranslationWindow() override;
};