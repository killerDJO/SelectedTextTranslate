#include "View\Content\Settings\Base\Dto\SettingsState.h"

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