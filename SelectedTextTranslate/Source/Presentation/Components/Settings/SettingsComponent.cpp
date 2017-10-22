#include "Presentation\Components\Settings\SettingsComponent.h"
#include "Presentation\MessageBus.h"

SettingsComponent::SettingsComponent(CommonContext* context, View* parentView)
    : Component<SettingsView>(context, new SettingsView(context, parentView, this))
{
    settingsProvider = context->Get<SettingsProvider>();
    settingsViewModel = new SettingsViewModel(settingsProvider->GetSettings());

    view->OnSaveSettings.Subscribe([context, this]
    {
        UpdateSettings(settingsViewModel->GetCurrentSettings());
    });

    view->OnCancelChanges.Subscribe([context, this]
    {
        settingsViewModel->CancelChanges();
        view->Render(true);
    });

    view->OnResetSettings.Subscribe([context, this]
    {
        context->Get<MessageBus>()->ShowConfirmDialog(L"Confirm settings reset", [this] { UpdateSettings(Settings()); });
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
    view->Render(true);
}

SettingsComponent::~SettingsComponent()
{
    delete settingsViewModel;
}