#pragma once
#include "BusinessLogic\Translation\TranslationService.h"
#include "Presentation\Framework\Views\View.h"
#include "Presentation\Components\Translation\TranslationView.h"
#include "Presentation\Components\Translation\TranslationViewModel.h"

class TranslationComponent : public Component<TranslationView>, public ModelHolder<TranslationViewModel*>
{
private:
    TranslationService* translationService;

    TranslationViewModel* translationViewModel;

    void ForceTranslation();
    void TranslateSuggestion();
    void RecreateViewModel(TranslateResult translateResult);

public:
    TranslationComponent(ServiceRegistry* serviceRegistry, View* parentView);

    void Translate(wstring input, bool incrementTranslationsCount);

    TranslationViewModel* GetModel() override;
};
