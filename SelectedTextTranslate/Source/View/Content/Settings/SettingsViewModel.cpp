#include "Services\Settings\Dto\Settings.h"
#include "View\Content\Settings\SettingsViewModel.h"

SettingsViewModel::SettingsViewModel(Settings settings)
{
    hotkeySettingsViewModel = new SettingsGroupViewModel<HotkeySettings>(settings.GetHotkeySettings(), L"Hotkeys");
}

SettingsGroupViewModel<HotkeySettings>* SettingsViewModel::GetHotkeySettingsViewModel() const
{
    return hotkeySettingsViewModel;
}

bool SettingsViewModel::HasChanges() const
{
    return hotkeySettingsViewModel->HasChanges();
}

bool SettingsViewModel::IsValid() const
{
    return hotkeySettingsViewModel->GetContentState() != SettingsGroupContentState::Invalid;
}

bool SettingsViewModel::AreDefaultSettings() const
{
    return GetCurrentSettings().EqualTo(Settings());
}

Settings SettingsViewModel::GetCurrentSettings() const
{
    return Settings(*hotkeySettingsViewModel->GetSettings());
}

void SettingsViewModel::CancelChanges() const
{
    hotkeySettingsViewModel->CancelChanges();
}

void SettingsViewModel::SetSettings(Settings settings) const
{
    hotkeySettingsViewModel->SetSettings(settings.GetHotkeySettings());
}

SettingsViewModel::~SettingsViewModel()
{
    delete hotkeySettingsViewModel;
}
