#pragma once
#include "View\Framework\View\Component.h"
#include "View\Content\Settings\SettingsView.h"
#include "Services\Settings\SettingsProvider.h"

class SettingsComponent : public Component<SettingsView>, public ModelHolder<Settings>
{
private:
    SettingsProvider* settingsProvider;

public:
    SettingsComponent(ViewContext* context, View* parentView);

    Settings GetModel() override;
    void UpdateSettings(Settings settings) const;
};