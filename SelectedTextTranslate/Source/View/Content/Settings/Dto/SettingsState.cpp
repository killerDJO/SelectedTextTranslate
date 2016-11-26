#include "View\Content\Settings\Dto\SettingsState.h"

SettingsState::SettingsState()
{
    this->hotkeySettingsGroupState = SettingsGroupState::Expanded;
}

SettingsGroupState SettingsState::GetHotkeySettingsGroupState() const
{
    return hotkeySettingsGroupState;
}

void SettingsState::SetHotkeySettingsGroupState(SettingsGroupState hotkeySettingsGroupState)
{
    this->hotkeySettingsGroupState = hotkeySettingsGroupState;
}

SettingsState::~SettingsState()
{
}
