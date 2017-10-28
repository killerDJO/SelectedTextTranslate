#include "Presentation\Components\Translation\TranslationComponent.h"
#include "Presentation\Framework\ViewModelsStore.h"

TranslationComponent::TranslationComponent(ServiceRegistry* serviceRegistry, View* parentView)
    : Component(new TranslationView(serviceRegistry->Get<CommonContext>(), parentView, this))
{
    translationService = serviceRegistry->Get<TranslationService>();
    viewModelsStore = serviceRegistry->Get<ViewModelsStore>();

    CurrentView->OnForceTranslation.Subscribe(bind(&TranslationComponent::ForceTranslation, this));
    CurrentView->OnTranslateSuggestion.Subscribe(bind(&TranslationComponent::TranslateSuggestion, this));
}

void TranslationComponent::Translate(wstring input, bool incrementTranslationsCount, bool forceTranslation) const
{
    viewModelsStore->Store(new TranslationViewModel(translationService->TranslateSentence(input, incrementTranslationsCount, forceTranslation)));
    Render();
}

void TranslationComponent::ForceTranslation()
{
    Translate(GetModel()->GetTranslateResult().GetSentence().GetInput(), false, true);
}

void TranslationComponent::TranslateSuggestion()
{
    Translate(GetModel()->GetTranslateResult().GetSentence().GetSuggestion(), false, true);
}

TranslationViewModel* TranslationComponent::GetModel()
{
    return viewModelsStore->Get<TranslationViewModel>();
}