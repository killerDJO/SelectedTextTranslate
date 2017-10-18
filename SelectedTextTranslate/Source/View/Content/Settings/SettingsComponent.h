#pragma once
#include "View\Framework\View\Component.h"
#include "View\Content\Settings\SettingsView.h"
#include "Services\Settings\SettingsProvider.h"

class SettingsComponent : Component<SettingsView>, ModelHolder<Settings>
{
private:
    SettingsProvider* settingsProvider;

public:
    SettingsComponent(ViewContext* context, View* parentView);

    Settings GetModel() override;
    void ResetSettings();
    void SaveSettings();
    void UpdateSettings(Settings settings);
};
