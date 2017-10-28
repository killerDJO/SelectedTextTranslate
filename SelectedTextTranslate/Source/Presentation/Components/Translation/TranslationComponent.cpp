#include "Presentation\Components\Translation\TranslationComponent.h"

TranslationComponent::TranslationComponent(ServiceRegistry* serviceRegistry, View* parentView)
    : Component( new TranslationView(serviceRegistry->Get<CommonContext>(), parentView, this))
{
    translationService = serviceRegistry->Get<TranslationService>();
    CurrentView->OnForceTranslation.Subscribe(bind(&TranslationComponent::ForceTranslation, this));
    CurrentView->OnTranslateSuggestion.Subscribe(bind(&TranslationComponent::TranslateSuggestion, this));
    translationViewModel = nullptr;
}

void TranslationComponent::Translate(wstring input, bool incrementTranslationsCount)
{
    RecreateViewModel(translationService->TranslateSentence(input, incrementTranslationsCount, false));
    Render();
}

void TranslationComponent::ForceTranslation()
{
    RecreateViewModel(translationService->TranslateSentence(translationViewModel->GetTranslateResult().GetSentence().GetInput(), false, true));
    Render();
}

void TranslationComponent::TranslateSuggestion()
{
    RecreateViewModel(translationService->TranslateSentence(translationViewModel->GetTranslateResult().GetSentence().GetSuggestion(), false, true));
    Render();
}

void TranslationComponent::RecreateViewModel(TranslateResult translateResult)
{
    if(translationViewModel != nullptr)
    {
        delete translationViewModel;
        translationViewModel = nullptr;
    }

    translationViewModel = new TranslationViewModel(translateResult);
}

TranslationViewModel* TranslationComponent::GetModel()
{
    return translationViewModel;
}