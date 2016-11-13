#pragma once
#include "View\Content\Base\ContentWindow.h"

class SettingsWindow : public ContentWindow
{
private:
    int SettingsWindow::RenderHotkeysEditControl(Renderer* renderer, wstring title, int curY);
    HFONT hotkeyControlFont;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    SettingsWindow(WindowContext* context, WindowDescriptor descriptor, wstring name, Window* parentWindow);
    ~SettingsWindow() override;

    void Initialize() override;
};