#pragma once
#include "View\Content\Base\ContentWindow.h"
#include "View\Content\Settings\Base\SettingsGroupWindow.h"

class SettingsWindow : public ContentWindow
{
private:
    int InitializeSettingsGroup(Renderer* renderer, SettingsGroupWindow* settingsGroup, int curY, int index);
    map<int, bool> stateMap;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    SettingsWindow(WindowContext* context, Window* parentWindow);
    ~SettingsWindow() override;

    void Initialize() override;
};