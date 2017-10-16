#pragma once
#include "Services\Translation\TranslationService.h"
#include "View\Framework\View\Views\View.h"
#include "View\Content\Translation\TranslationView.h"
#include "Services\Translation\TextPlayer.h"

class TranslationComponent : public Component<TranslationView>, public ModelHolder<TranslateResult>
{
private:
    TranslationService* translationService;

    TranslateResult translateResult;

    void ForceTranslation();
    void TranslateSuggestion();
    void PlayText();

public:
    TranslationComponent(ViewContext* context, View* parentView, TranslationService* translationService, TextPlayer* textPlayer);

    void Translate(wstring input, bool incrementTranslationsCount);

    TranslateResult GetModel() override;
};