#include "Presentation\Components\Settings\SettingsComponent.h"
#include "Presentation\MessageBus.h"

SettingsComponent::SettingsComponent(ServiceRegistry* serviceRegistry, View* parentView)
    : Component<SettingsView>(new SettingsView(serviceRegistry->Get<ViewContext>(), parentView, this, this))
{
    settingsProvider = serviceRegistry->Get<SettingsProvider>();
    viewModelsStore = serviceRegistry->Get<ViewModelsStore>();

    viewModelsStore->EnsureDefault(new SettingsViewModel(settingsProvider->GetSettings()));

    CurrentView->OnSaveSettings.Subscribe([this]
    {
        UpdateSettings(GetModel()->GetCurrentSettings());
    });

    CurrentView->OnCancelChanges.Subscribe([this]
    {
        GetModel()->CancelChanges();
        CurrentView->Render(true);
    });

    CurrentView->OnResetSettings.Subscribe([serviceRegistry, this]
    {
        serviceRegistry->Get<MessageBus>()->OnConfirmRequested.Notify(L"Confirm settings reset", [this] { UpdateSettings(Settings()); });
    });
}

SettingsViewModel* SettingsComponent::GetModel()
{
    return viewModelsStore->Get<SettingsViewModel>();
}

void SettingsComponent::UpdateSettings(Settings settings)
{
    settingsProvider->UpdateSettings(settings);
    GetModel()->SetSettings(settings);
    CurrentView->Render(true);
}