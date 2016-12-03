#pragma once
#include "View\Content\Settings\Base\SettingsGroupWindow.h"
#include "View\Framework\ModelHolder.h"

class HotkeySettingsWindow : public SettingsGroupWindow, public ModelHolder<HotkeySettings>
{
private:
    RenderResult RenderHotkeyEditControl(RenderDescriptor renderDescriptor, wstring title, int hotkey, function<void(DWORD)> hotkeySetter);

protected:
    void RenderSettingsContent(RenderDescriptor renderDescriptor) override;

public:
    HotkeySettingsWindow(WindowContext* context, Window* parentWindow);
    ~HotkeySettingsWindow() override;

    Subscribeable<HotkeySettings> OnSettingsChanged;
};
