#pragma once
#include "View\Content\Settings\Base\Enums\SettingsGroupState.h"

class SettingsState
{
private:
    SettingsGroupState hotkeySettingsGroupState;

public:
    SettingsState();
    ~SettingsState();

    SettingsGroupState GetHotkeySettingsGroupState() const;
    void SetHotkeySettingsGroupState(SettingsGroupState hotkeySettingsGroupState);
};