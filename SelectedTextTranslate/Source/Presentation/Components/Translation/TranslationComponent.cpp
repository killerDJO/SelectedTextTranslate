#include "Presentation\Components\Translation\TranslationComponent.h"

TranslationComponent::TranslationComponent(CommonContext* context, View* parentView)
    : Component(context, new TranslationView(context, parentView, this))
{
    this->translationService = context->Get<TranslationService>();
    CurrentView->OnForceTranslation.Subscribe(bind(&TranslationComponent::ForceTranslation, this));
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

TranslateResult TranslationComponent::GetModel()
{
    return translateResult;
}