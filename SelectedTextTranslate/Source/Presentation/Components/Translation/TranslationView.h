#pragma once
#include "Presentation\Components\Translation\Header\HeaderComponent.h"
#include "Presentation\Components\Translation\TranslationViewModel.h"
#include "Presentation\Components\Translation\Content\TranslateResultComponent.h"

class TranslationView : public ComponentView<TranslationViewModel*>
{
private:
    int headerHeight;
    int separatorHeight;

    HeaderComponent* headerComponent;
    TranslateResultComponent* translateResultComponent;

    void RenderSeparator(Renderer* renderer, int width) const;

protected:
    Size RenderContent(Renderer* renderer, TranslationViewModel* model) override;

public:
    TranslationView(CommonContext* context, View* parentView, ModelHolder<TranslationViewModel*>* modelHolder);

    void Initialize() override;
    void Resize() override;

    Subscribeable<> OnForceTranslation;
    Subscribeable<> OnTranslateSuggestion;
};
