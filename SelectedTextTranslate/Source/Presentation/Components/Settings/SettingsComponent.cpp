#include "Presentation\Components\Settings\SettingsComponent.h"
#include "Presentation\MessageBus.h"

SettingsComponent::SettingsComponent(CommonContext* context, View* parentView)
    : Component<SettingsView>(context, new SettingsView(context, parentView, this))
{
    settingsProvider = context->Get<SettingsProvider>();
    settingsViewModel = new SettingsViewModel(settingsProvider->GetSettings());

    CurrentView->OnSaveSettings.Subscribe([context, this]
    {
        UpdateSettings(settingsViewModel->GetCurrentSettings());
    });

    CurrentView->OnCancelChanges.Subscribe([context, this]
    {
        settingsViewModel->CancelChanges();
        CurrentView->Render(true);
    });

    CurrentView->OnResetSettings.Subscribe([context, this]
    {
        context->Get<MessageBus>()->OnConfirmRequested.Notify(L"Confirm settings reset", [this] { UpdateSettings(Settings()); });
    });
}

SettingsViewModel* SettingsComponent::GetModel()
{
    return settingsViewModel;
}

void SettingsComponent::UpdateSettings(Settings settings) const
{
    settingsProvider->UpdateSettings(settings);
    settingsViewModel->SetSettings(settings);
    CurrentView->Render(true);
}

SettingsComponent::~SettingsComponent()
{
    delete settingsViewModel;
}