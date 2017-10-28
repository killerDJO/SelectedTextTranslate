#include "Presentation\Components\Main\MainComponent.h"
#include "Presentation\Providers\TrayIcon.h"
#include "BusinessLogic\Translation\TextExtractor.h"

MainComponent::MainComponent(ServiceRegistry* serviceRegistry)
    : Component(new MainView(serviceRegistry->Get<ViewContext>(), this))
{
    hotkeysRegistry = serviceRegistry->Get<HotkeysRegistry>();
    textExtractor = serviceRegistry->Get<TextExtractor>();
    textPlayer = serviceRegistry->Get<TextPlayer>();
    messageBus = serviceRegistry->Get<MessageBus>();
    translationService = serviceRegistry->Get<TranslationService>();
    viewModelsStore = serviceRegistry->Get<ViewModelsStore>();

    messageBus->OnPlaySelectedText.Subscribe(bind(&MainComponent::PlaySelectedText, this));
    messageBus->OnShowDictionary.Subscribe(bind(&MainComponent::ShowApplicatonView, this, ApplicationViews::Dictionary));
    messageBus->OnShowSettings.Subscribe(bind(&MainComponent::ShowApplicatonView, this, ApplicationViews::Settings));
    messageBus->OnTranslateSelectedText.Subscribe(bind(&MainComponent::TranslateSelectedText, this));

    CurrentView->OnHotkey.Subscribe(bind(&MainComponent::ProcessHotkey, this, placeholders::_1));
    CurrentView->OnVisibilityChanged.Subscribe(bind(&MainComponent::ProcessVisibilityChange, this, placeholders::_1));
}

void MainComponent::SetLayout(LayoutDescriptor layout)
{
    Component::SetLayout(layout);
    viewModelsStore->Store(new MainViewModel(ApplicationViews::None, layout));
}

MainViewModel* MainComponent::GetModel()
{
    return viewModelsStore->Get<MainViewModel>();
}

void MainComponent::PlaySelectedText() const
{
    wstring selectedText = textExtractor->GetSelectedText();
    TranslateResult translateResult = translationService->TranslateSentence(selectedText, false, false);
    textPlayer->PlayText(translateResult.GetSentence().GetOrigin());
}

void MainComponent::TranslateSelectedText() const
{
    wstring selectedText = textExtractor->GetSelectedText();
    CurrentView->Translate(selectedText);
}

void MainComponent::ShowApplicatonView(ApplicationViews applicationView)
{
    GetModel()->SetApplicationView(applicationView);
    CurrentView->Render();
}

void MainComponent::ProcessHotkey(int hotkey) const
{
    hotkeysRegistry->ProcessHotkey(hotkey);
}

void MainComponent::ProcessVisibilityChange(bool isVisible)
{
    const double ScaleFactor = 0.05;
    if (isVisible)
    {
        hotkeysRegistry->RegisterZoomInHotkey(
            CurrentView->GetHandle(),
            [=]() -> void { CurrentView->Scale(ScaleFactor); });

        hotkeysRegistry->RegisterZoomOutHotkey(
            CurrentView->GetHandle(),
            [=]() -> void { CurrentView->Scale(-ScaleFactor); });
    }
    else
    {
        hotkeysRegistry->UnregisterZoomInHotkey(CurrentView->GetHandle());
        hotkeysRegistry->UnregisterZoomOutHotkey(CurrentView->GetHandle());
    }
}

MainComponent::~MainComponent()
{
    delete CurrentView;
}