#pragma once
#include "View\Framework\ModelHolder.h"
#include "View\Content\Settings\Base\SettingGroupComponent.h"
#include "Services\Settings\Dto\HotkeySettings.h"
#include "View\Content\Settings\Hotkeys\HotkeySettingsView.h"

class HotkeySettingsComponent : public SettingGroupComponent<HotkeySettings, HotkeySettingsView>
{
public:
    HotkeySettingsComponent(ViewContext* context, View* parentView, ModelHolder<SettingsGroupViewModel<HotkeySettings>*>* modelHolder);

    bool IsValid() override;
};
