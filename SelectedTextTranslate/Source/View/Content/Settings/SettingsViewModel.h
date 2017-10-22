#pragma once
#include "View\Content\Settings\Base\Dto\SettingsGroupViewModel.h"
#include "Services\Settings\Dto\HotkeySettings.h"
#include "Services\Settings\Dto\Settings.h"

class SettingsViewModel
{
private:
    SettingsGroupViewModel<HotkeySettings>* hotkeySettingsViewModel;

public:
    SettingsViewModel(Settings settings);
    ~SettingsViewModel();

    SettingsGroupViewModel<HotkeySettings>* GetHotkeySettingsViewModel() const;

    bool HasChanges() const;
    bool IsValid() const;
    bool AreDefaultSettings() const;

    Settings GetCurrentSettings() const;

    void CancelChanges() const;
    void SetSettings(Settings settings) const;
};