#include "Presentation\Components\Main\MainComponent.h"
#include "Presentation\Providers\TrayIcon.h"
#include "BusinessLogic\Translation\TextExtractor.h"

MainComponent::MainComponent(CommonContext* context)
    : Component(context, new MainView(context, this))
{
    hotkeysRegistry = context->Get<HotkeysRegistry>();
    textExtractor = context->Get<TextExtractor>();
    textPlayer = context->Get<TextPlayer>();
    messageBus = context->Get<MessageBus>();

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
    viewModel = new MainViewModel(ApplicationViews::None, layout);
}

MainViewModel* MainComponent::GetModel()
{
    return viewModel;
}

void MainComponent::PlaySelectedText() const
{
    wstring selectedText = textExtractor->GetSelectedText();
    TranslateResult translateResult = Context->Get<TranslationService>()->TranslateSentence(selectedText, false, false);
    textPlayer->PlayText(translateResult.GetSentence().GetOrigin());
}

void MainComponent::TranslateSelectedText() const
{
    wstring selectedText = textExtractor->GetSelectedText();
    CurrentView->Translate(selectedText);
}

void MainComponent::ShowApplicatonView(ApplicationViews applicationView) const
{
    viewModel->SetApplicationView(applicationView);
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
    if(viewModel != nullptr)
    {
        delete viewModel;
    }
}