#pragma once
#include "Presentation\Framework\ModelHolder.h"
#include "Presentation\Components\Settings\Base\SettingsGroupComponent.h"
#include "BusinessLogic\Settings\Dto\HotkeySettings.h"
#include "Presentation\Components\Settings\Hotkeys\HotkeySettingsView.h"

class HotkeySettingsComponent : public SettingsGroupComponent<HotkeySettings, HotkeySettingsView>
{
public:
    HotkeySettingsComponent(ServiceRegistry* serviceRegistry, View* parentView, ModelHolder<SettingsGroupViewModel<HotkeySettings>*>* modelHolder);

    bool IsValid() override;
};
