#pragma once
#include "View\Content\Settings\Base\SettingsGroupWindow.h"

class HotkeySettingsWindow : public SettingGroupWindow
{
private:
    int RenderHotkeysEditControl(Renderer* renderer, wstring title, int curX, int curY);

protected:
    void RenderSettingsContent(Renderer* renderer, Point contentPosition) override;

public:
    HotkeySettingsWindow(WindowContext* context, Point position, int width, wstring name, Window* parentWindow, bool isCollapsed, int titleLineHeight);
    ~HotkeySettingsWindow() override;

    void Initialize() override;
};
