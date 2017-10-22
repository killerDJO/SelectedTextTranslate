#include "View\Framework\ModelHolder.h"
#include "View\Framework\View\Views\View.h"
#include "View\Content\Settings\Hotkeys\HotkeySettingsComponent.h"

HotkeySettingsComponent::HotkeySettingsComponent(CommonContext* context, View* parentView, ModelHolder<SettingsGroupViewModel<HotkeySettings>*>* modelHolder)
    : SettingsGroupComponent<HotkeySettings, HotkeySettingsView>(context, new HotkeySettingsView(context, parentView, modelHolder), modelHolder)
{
}

bool HotkeySettingsComponent::IsValid()
{
    bool isValid = true;

    HotkeySettings settings = *modelHolder->GetModel()->GetSettings();
    vector<DWORD> hotkeys = vector<DWORD>();
    hotkeys.push_back(settings.GetTranslateHotkey());
    hotkeys.push_back(settings.GetPlayTextHotkey());
    hotkeys.push_back(settings.GetZoomInHotkey());
    hotkeys.push_back(settings.GetZoomOutHotkey());

    for (size_t i = 0; i < hotkeys.size(); ++i)
    {
        for (size_t j = i + 1; j < hotkeys.size(); ++j)
        {
            isValid = isValid && hotkeys[i] != hotkeys[j];
        }
    }

    return isValid;
}