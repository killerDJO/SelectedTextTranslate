#include "View\Framework\ModelHolder.h"
#include "View\Framework\View\Views\View.h"
#include "View\Content\Settings\Hotkeys\HotkeySettingsComponent.h"

HotkeySettingsComponent::HotkeySettingsComponent(ViewContext* context, View* parentView, ModelHolder<HotkeySettings>* modelHolder)
    : SettingGroupComponent<HotkeySettings>(context, parentView, modelHolder)
{
    this->title = L"Hotkeys";
}

bool HotkeySettingsComponent::IsValid() const
{
    bool isValid = true;

    HotkeySettings settings = modelHolder->GetModel();
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

bool HotkeySettingsComponent::HasChanges() const
{
    return modelHolder->GetModel().EqualTo(settingsGroupState.GetSettings());
}
