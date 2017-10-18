#include "View\Content\Settings\SettingsComponent.h"

SettingsComponent::SettingsComponent(ViewContext* context, View* parentView)
    : Component<SettingsView>(context, new SettingsView(context, parentView, this))
{
    this->settingsProvider = context->Get<SettingsProvider>();
    view->OnSaveSettings.Subscribe(bind(&SettingsComponent::UpdateSettings, this, placeholders::_1));
    view->OnResetSettings.Subscribe(bind(&SettingsComponent::UpdateSettings, this, Settings()));
}

Settings SettingsComponent::GetModel()
{
    return settingsProvider->GetSettings();
}

void SettingsComponent::UpdateSettings(Settings settings) const
{
    settingsProvider->UpdateSettings(settings);
    view->Render(true);
}