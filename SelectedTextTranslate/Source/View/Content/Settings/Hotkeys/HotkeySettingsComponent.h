#pragma once
#include "View\Framework\ModelHolder.h"
#include "View\Content\Settings\Base\SettingsGroupComponent.h"
#include "Services\Settings\Dto\HotkeySettings.h"
#include "View\Content\Settings\Hotkeys\HotkeySettingsView.h"

class HotkeySettingsComponent : public SettingsGroupComponent<HotkeySettings, HotkeySettingsView>
{
public:
    HotkeySettingsComponent(CommonContext* context, View* parentView, ModelHolder<SettingsGroupViewModel<HotkeySettings>*>* modelHolder);

    bool IsValid() override;
};
