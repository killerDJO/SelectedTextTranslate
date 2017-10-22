#pragma once
#include "View\Controls\Inputs\HotKeyInputWindow.h"
#include "View\Content\Settings\Base\SettingsGroupView.h"
#include "Services\Settings\Dto\HotkeySettings.h"

class HotkeySettingsView : public SettingsGroupView<HotkeySettings>
{
private:
    int controlsMargin;

    Font* font;
    vector<HotKeyInputWindow*> hotkeyInputWindows;

    RenderResult RenderHotkeyEditControl(HotkeySettings* settings, RenderDescriptor renderDescriptor, wstring title, int hotkey, function<void(HotkeySettings*, DWORD)> hotkeySetter);

    void SetHotkeyInputsValiationState();

protected:
    void RenderSettingsContent(RenderDescriptor renderDescriptor, HotkeySettings* settings) override;

public:
    HotkeySettingsView(ViewContext* context, View* parentView, ModelHolder<SettingsGroupViewModel<HotkeySettings>*>* modelHolder);
    ~HotkeySettingsView() override;
};