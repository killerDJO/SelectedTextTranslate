#pragma once
#include "BusinessLogic\Translation\TranslationService.h"
#include "Presentation\Framework\Views\View.h"
#include "Presentation\Components\Translation\TranslationView.h"

class TranslationComponent : public Component<TranslationView>, public ModelHolder<TranslateResult>
{
private:
    TranslationService* translationService;

    TranslateResult translateResult;

    void ForceTranslation();
    void TranslateSuggestion();

public:
    TranslationComponent(CommonContext* context, View* parentView);

    void Translate(wstring input, bool incrementTranslationsCount);

    TranslateResult GetModel() override;
};