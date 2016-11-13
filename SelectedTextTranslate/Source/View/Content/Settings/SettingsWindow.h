#pragma once
#include "View\Content\Base\ContentWindow.h"

class SettingsWindow : public ContentWindow
{
protected:
    Size RenderContent(Renderer* renderer) override;

public:
    SettingsWindow(WindowContext* context, WindowDescriptor descriptor, wstring name, Window* parentWindow);
    ~SettingsWindow() override;
};