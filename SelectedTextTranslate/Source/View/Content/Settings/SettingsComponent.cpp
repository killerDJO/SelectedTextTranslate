#include "View\Content\Settings\SettingsComponent.h"
#include "View\Framework\MessageBus.h"

SettingsComponent::SettingsComponent(ViewContext* context, View* parentView)
    : Component<SettingsView>(context, new SettingsView(context, parentView, this))
{
    this->settingsProvider = context->Get<SettingsProvider>();
    view->OnSaveSettings.Subscribe(bind(&SettingsComponent::UpdateSettings, this, placeholders::_1));
    view->OnResetSettings.Subscribe([context, this]
    {
        context->Get<MessageBus>()->ShowConfirmDialog(L"Confirm settings reset", [this] { UpdateSettings(Settings()); });
    });
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