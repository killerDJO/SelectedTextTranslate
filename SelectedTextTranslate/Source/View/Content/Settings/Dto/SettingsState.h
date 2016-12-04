#pragma once
#include "View\Content\Settings\Base\Enums\SettingsGroupVisibilityState.h"

class SettingsState
{
private:
    SettingsGroupVisibilityState hotkeySettingsGroupVisibilityState;

public:
    SettingsState();
    ~SettingsState();

    SettingsGroupVisibilityState GetHotkeySettingsGroupVisibilityState() const;
    void SetHotkeySettingsGroupVisibilityState(SettingsGroupVisibilityState hotkeySettingsGroupState);
};
