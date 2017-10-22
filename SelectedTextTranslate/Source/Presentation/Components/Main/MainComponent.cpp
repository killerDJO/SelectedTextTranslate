#include "Presentation\Components\Main\MainComponent.h"
#include "Presentation\Providers\TrayIconProvider.h"
#include "BusinessLogic\Translation\TextExtractor.h"

MainComponent::MainComponent(CommonContext* context)
    : Component(context, new MainView(context))
{
    hotkeyProvider = context->Get<HotkeyProvider>();
    textExtractor = context->Get<TextExtractor>();
    textPlayer = context->Get<TextPlayer>();

    TrayIconProvider* trayIconProvider = context->Get<TrayIconProvider>();
    trayIconProvider->OnExit.Subscribe(bind(&MainComponent::Exit, this));
    trayIconProvider->OnPlaySelectedText.Subscribe(bind(&MainComponent::PlaySelectedText, this));
    trayIconProvider->OnShowDictionary.Subscribe(bind(&MainComponent::ShowApplicatonView, this, ApplicationViews::Dictionary));
    trayIconProvider->OnShowSettings.Subscribe(bind(&MainComponent::ShowApplicatonView, this, ApplicationViews::Settings));
    trayIconProvider->OnTranslateSelectedText.Subscribe(bind(&MainComponent::TranslateSelectedText, this));
    trayIconProvider->OnSuspend.Subscribe(bind(&MainComponent::Suspend, this));
    trayIconProvider->OnEnable.Subscribe(bind(&MainComponent::Enable, this));
}

void MainComponent::PlaySelectedText() const
{
    wstring selectedText = textExtractor->GetSelectedText();
    TranslateResult translateResult = context->Get<TranslationService>()->TranslateSentence(selectedText, false, false);
    textPlayer->PlayText(translateResult.GetSentence().GetOrigin());
}

void MainComponent::TranslateSelectedText() const
{
    wstring selectedText = textExtractor->GetSelectedText();
    view->Translate(selectedText);
}

void MainComponent::ShowApplicatonView(ApplicationViews applicationView) const
{
    view->SetApplicationView(applicationView);
    view->Render();
}

void MainComponent::Suspend() const
{
    trayIconProvider->SetSuspendedState();
}

void MainComponent::Enable() const
{
    trayIconProvider->SetEnabledState();
}

void MainComponent::Exit() const
{
    PostQuitMessage(0);
}

void MainComponent::ProcessHotkey(int hotkey) const
{
    hotkeyProvider->ProcessHotkey(hotkey);
}

void MainComponent::ProcessVisibilityChange(bool isVisible)
{
    if (isVisible)
    {
        hotkeyProvider->RegisterZoomInHotkey(
            view->GetHandle(),
            [=]() -> void { view->Scale(0.05); });

        hotkeyProvider->RegisterZoomOutHotkey(
            view->GetHandle(),
            [=]() -> void { view->Scale(-0.05); });
    }
    else
    {
        hotkeyProvider->UnregisterZoomInHotkey(view->GetHandle());
        hotkeyProvider->UnregisterZoomOutHotkey(view->GetHandle());
    }
}