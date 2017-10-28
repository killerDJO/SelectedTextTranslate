#include "Presentation\Components\Settings\SettingsComponent.h"
#include "Presentation\MessageBus.h"

SettingsComponent::SettingsComponent(ServiceRegistry* serviceRegistry, View* parentView)
    : Component<SettingsView>(new SettingsView(serviceRegistry->Get<CommonContext>(), parentView, this))
{
    settingsProvider = serviceRegistry->Get<SettingsProvider>();
    settingsViewModel = new SettingsViewModel(settingsProvider->GetSettings());

    CurrentView->OnSaveSettings.Subscribe([this]
    {
        UpdateSettings(settingsViewModel->GetCurrentSettings());
    });

    CurrentView->OnCancelChanges.Subscribe([this]
    {
        settingsViewModel->CancelChanges();
        CurrentView->Render(true);
    });

    CurrentView->OnResetSettings.Subscribe([serviceRegistry, this]
    {
        serviceRegistry->Get<MessageBus>()->OnConfirmRequested.Notify(L"Confirm settings reset", [this] { UpdateSettings(Settings()); });
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