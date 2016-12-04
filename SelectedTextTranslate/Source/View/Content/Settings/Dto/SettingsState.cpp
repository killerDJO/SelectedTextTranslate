#include "View\Content\Settings\Dto\SettingsState.h"

SettingsState::SettingsState()
{
    this->hotkeySettingsGroupVisibilityState = SettingsGroupVisibilityState::Expanded;
}

SettingsGroupVisibilityState SettingsState::GetHotkeySettingsGroupVisibilityState() const
{
    return hotkeySettingsGroupVisibilityState;
}

void SettingsState::SetHotkeySettingsGroupVisibilityState(SettingsGroupVisibilityState hotkeySettingsGroupVisibilityState)
{
    this->hotkeySettingsGroupVisibilityState = hotkeySettingsGroupVisibilityState;
}

SettingsState::~SettingsState()
{
}
