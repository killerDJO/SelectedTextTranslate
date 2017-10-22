#pragma once
#include "Presentation\Components\Settings\Base\SettingsGroupViewModel.h"
#include "BusinessLogic\Settings\Dto\HotkeySettings.h"
#include "BusinessLogic\Settings\Dto\Settings.h"

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