#pragma once
#include "View\Framework\View\Component.h"
#include "View\Content\Settings\SettingsView.h"
#include "Services\Settings\SettingsProvider.h"
#include "View\Content\Settings\SettingsViewModel.h"

class SettingsComponent : public Component<SettingsView>, public ModelHolder<SettingsViewModel*>
{
private:
    SettingsProvider* settingsProvider;
    SettingsViewModel* settingsViewModel;

public:
    SettingsComponent(ViewContext* context, View* parentView);
    ~SettingsComponent();

    SettingsViewModel* GetModel() override;
    void UpdateSettings(Settings settings) const;
};