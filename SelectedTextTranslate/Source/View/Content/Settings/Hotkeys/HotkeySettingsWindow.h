#pragma once
#include "View\Content\Settings\Base\SettingsGroupWindow.h"
#include "View\Controls\Inputs\HotKeyInputWindow.h"
#include "View\Framework\ModelHolder.h"
#include "Services\Settings\Dto\HotkeySettings.h"

class HotkeySettingsWindow : public SettingsGroupWindow, public ModelHolder<HotkeySettings>
{
private:
    Font* font;
    HotkeySettings globalModel;
    vector<HotKeyInputWindow*> hotkeyInputWindows;

    RenderResult RenderHotkeyEditControl(RenderDescriptor renderDescriptor, wstring title, int hotkey, function<void(DWORD)> hotkeySetter);

    void ValidateHotkeyInputs();
    bool IsModelValid() const;

protected:
    void RenderSettingsContent(RenderDescriptor renderDescriptor) override;

public:
    HotkeySettingsWindow(WindowContext* context, Window* parentWindow);
    ~HotkeySettingsWindow() override;

    void ComputeContentState() override;
    void SetModel(HotkeySettings model) override;

    Subscribeable<HotkeySettings> OnSettingsChanged;
};
