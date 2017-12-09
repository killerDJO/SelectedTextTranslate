#include "Presentation\Components\Translation\TranslationComponent.h"
#include "Presentation\Framework\ViewModelsStore.h"

TranslationComponent::TranslationComponent(ServiceRegistry* serviceRegistry, View* parentView)
    : Component(new TranslationView(serviceRegistry->Get<ViewContext>(), parentView, this, this))
{
    translationService = serviceRegistry->Get<TranslationService>();
    viewModelsStore = serviceRegistry->Get<ViewModelsStore>();
    textExtractor = serviceRegistry->Get<TextExtractor>();
    textPlayer = serviceRegistry->Get<TextPlayer>();
    messageBus = serviceRegistry->Get<MessageBus>();

    CurrentView->OnForceTranslation.Subscribe(function<void()>(bind(&TranslationComponent::ForceTranslation, this)));
    CurrentView->OnTranslateSuggestion.Subscribe(bind(&TranslationComponent::TranslateSuggestion, this));

    RegisterForDispose(messageBus->OnTranslateText.Subscribe(bind(&TranslationComponent::Translate, this, placeholders::_1, placeholders::_2, false)));
    RegisterForDispose(messageBus->OnTranslateSelectedText.Subscribe(bind(&TranslationComponent::TranslateSelectedText, this)));
    RegisterForDispose(messageBus->OnPlaySelectedText.Subscribe(bind(&TranslationComponent::PlaySelectedText, this)));
}

void TranslationComponent::PlaySelectedText() const
{
    wstring selectedText = textExtractor->GetSelectedText();
    TranslateResult translateResult = translationService->TranslateSentence(selectedText, false, false);
    textPlayer->PlayText(translateResult.GetSentence().GetOrigin());
}

void TranslationComponent::TranslateSelectedText() const
{
    wstring selectedText = textExtractor->GetSelectedText();
    Translate(selectedText, true, false);
}

void TranslationComponent::Translate(wstring input, bool incrementTranslationsCount, bool forceTranslation) const
{
    viewModelsStore->Store(new TranslationViewModel(translationService->TranslateSentence(input, incrementTranslationsCount, forceTranslation)));
    messageBus->OnShowTranslation();
}

void TranslationComponent::ForceTranslation()
{
    Translate(GetModel()->GetTranslateResult().GetSentence().GetInput(), false, true);
    Render();
}

void TranslationComponent::TranslateSuggestion()
{
    Translate(GetModel()->GetTranslateResult().GetSentence().GetSuggestion(), false, true);
    Render();
}

TranslationViewModel* TranslationComponent::GetModel()
{
    return viewModelsStore->Get<TranslationViewModel>();
}
