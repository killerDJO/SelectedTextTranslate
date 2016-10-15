#pragma once
#include "View\Content\Base\ContentWindow.h"

class ErrorWindow : public ContentWindow
{
protected:
    Size RenderContent(Renderer* renderer) override;

public:
    ErrorWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow, AppController* appController);
    ~ErrorWindow() override;
};