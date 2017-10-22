#pragma once
#include "Services\Translation\TranslationService.h"
#include "View\Framework\View\Views\View.h"
#include "View\Content\Translation\TranslationView.h"

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