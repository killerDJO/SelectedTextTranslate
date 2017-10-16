#include "View\Content\Translation\TranslationComponent.h"

TranslationComponent::TranslationComponent(ViewContext* context, View* parentView, TranslationService* translationService)
    : Component(new TranslationView(context, parentView, this))
{
    this->translationService = translationService;
    view->OnForceTranslation.Subscribe(bind(&TranslationComponent::ForceTranslation, this));
}

void TranslationComponent::Translate(wstring input, bool incrementTranslationsCount)
{
    translateResult = translationService->TranslateSentence(input, incrementTranslationsCount, false);
    Render();
}

void TranslationComponent::ForceTranslation()
{
    translateResult = translationService->TranslateSentence(translateResult.GetSentence().GetInput(), false, true);
    Render();
}

void TranslationComponent::TranslateSuggestion()
{
    translateResult = translationService->TranslateSentence(translateResult.GetSentence().GetInput(), false, true);
    Render();
}

void TranslationComponent::PlayText()
{
}

TranslateResult TranslationComponent::GetModel()
{
    return translateResult;
}