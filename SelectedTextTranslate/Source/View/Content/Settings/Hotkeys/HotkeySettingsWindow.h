#pragma once
#include "View\Content\Settings\Base\SettingsGroupWindow.h"
#include "View\Framework\ModelHolder.h"

class HotkeySettingsWindow : public SettingsGroupWindow, public ModelHolder<HotkeySettings>
{
private:
    int RenderHotkeyEditControl(Renderer* renderer, wstring title, int curX, int curY, int hotkey, function<void(DWORD)> hotkeySetter);

protected:
    void RenderSettingsContent(Renderer* renderer, Point contentPosition) override;

public:
    HotkeySettingsWindow(WindowContext* context, Window* parentWindow);
    ~HotkeySettingsWindow() override;

    Subscribeable<HotkeySettings> OnSettingsChanged;
};
