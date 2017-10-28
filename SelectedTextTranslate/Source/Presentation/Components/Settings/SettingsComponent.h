#pragma once
#include "Presentation\Framework\Components\Component.h"
#include "Presentation\Components\Settings\SettingsView.h"
#include "BusinessLogic\Settings\SettingsProvider.h"
#include "Presentation\Components\Settings\SettingsViewModel.h"

class SettingsComponent : public Component<SettingsView>, public ModelHolder<SettingsViewModel*>
{
private:
    SettingsProvider* settingsProvider;
    SettingsViewModel* settingsViewModel;

public:
    SettingsComponent(CommonContext* context, View* parentView);
    ~SettingsComponent();

    SettingsViewModel* GetModel() override;
    void UpdateSettings(Settings settings) const;
};