#pragma once
#include "Presentation\Framework\Components\Component.h"
#include "Presentation\Components\Settings\SettingsView.h"
#include "BusinessLogic\Settings\SettingsProvider.h"
#include "Presentation\Components\Settings\SettingsViewModel.h"
#include "Presentation\Framework\ViewModelsStore.h"

class SettingsComponent : public Component<SettingsView>, public ModelHolder<SettingsViewModel*>
{
private:
    SettingsProvider* settingsProvider;
    ViewModelsStore* viewModelsStore;

public:
    SettingsComponent(ServiceRegistry* serviceRegistry, View* parentView);

    SettingsViewModel* GetModel() override;
    void UpdateSettings(Settings settings);
};