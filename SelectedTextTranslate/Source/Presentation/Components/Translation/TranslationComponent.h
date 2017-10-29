#pragma once
#include "BusinessLogic\Translation\TranslationService.h"
#include "Presentation\Framework\Views\View.h"
#include "Presentation\Components\Translation\TranslationView.h"
#include "Presentation\Components\Translation\TranslationViewModel.h"
#include "Presentation\Framework\ViewModelsStore.h"
#include "BusinessLogic\Translation\TextExtractor.h"

class TranslationComponent : public Component<TranslationView>, public ModelHolder<TranslationViewModel*>
{
private:
    TranslationService* translationService;
    TextExtractor* textExtractor;
    TextPlayer* textPlayer;
    ViewModelsStore* viewModelsStore;
    MessageBus* messageBus;

    void ForceTranslation();
    void TranslateSuggestion();

    void PlaySelectedText() const;
    void TranslateSelectedText() const;

public:
    TranslationComponent(ServiceRegistry* serviceRegistry, View* parentView);

    void Translate(wstring input, bool incrementTranslationsCount, bool forceTranslation = false) const;

    TranslationViewModel* GetModel() override;
};
