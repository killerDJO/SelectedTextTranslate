#include "Presentation\Framework\ModelHolder.h"
#include "Presentation\Framework\Views\View.h"
#include "Presentation\Components\Settings\Hotkeys\HotkeySettingsComponent.h"

HotkeySettingsComponent::HotkeySettingsComponent(ServiceRegistry* serviceRegistry, View* parentView, ModelHolder<SettingsGroupViewModel<HotkeySettings>*>* modelHolder)
    : SettingsGroupComponent<HotkeySettings, HotkeySettingsView>(new HotkeySettingsView(serviceRegistry->Get<CommonContext>(), parentView, modelHolder), modelHolder)
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