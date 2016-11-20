#pragma once
#include "View\Content\Settings\Base\SettingsGroupWindow.h"

class HotkeySettingsWindow : public SettingsGroupWindow
{
private:
    int RenderHotkeysEditControl(Renderer* renderer, wstring title, int curX, int curY);

protected:
    void RenderSettingsContent(Renderer* renderer, Point contentPosition) override;

public:
    HotkeySettingsWindow(WindowContext* context, Window* parentWindow);
    ~HotkeySettingsWindow() override;

    void Initialize() override;
};
