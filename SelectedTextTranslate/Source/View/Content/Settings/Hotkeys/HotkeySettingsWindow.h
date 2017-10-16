#pragma once
#include "View\Controls\Inputs\HotKeyInputWindow.h"
#include "View\Content\Settings\Base\SettingsGroupWindow.h"

class HotkeySettingsWindow : public SettingsGroupWindow
{
private:
    int controlsMargin;

    Font* font;
    vector<HotKeyInputWindow*> hotkeyInputWindows;

    RenderResult RenderHotkeyEditControl(RenderDescriptor renderDescriptor, wstring title, int hotkey, function<void(HotkeySettings*, DWORD)> hotkeySetter);

    void SetHotkeyInputsValiationState();
    bool IsModelValid() const;

protected:
    void RenderSettingsContent(RenderDescriptor renderDescriptor) override;

public:
    HotkeySettingsWindow(ViewContext* context, View* parentWindow);
    ~HotkeySettingsWindow() override;

    void ComputeContentState() override;
};
