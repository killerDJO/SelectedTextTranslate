#pragma once
#include "Presentation\Components\Translation\Header\HeaderComponent.h"
#include "Presentation\Components\Translation\TranslationViewModel.h"
#include "Presentation\Components\Translation\Content\TranslateResultComponent.h"

class TranslationView : public ChildComponentView<TranslationViewModel*>
{
private:
    int headerHeight;
    int separatorHeight;

    HeaderComponent* headerComponent;
    TranslateResultComponent* translateResultComponent;

    void RenderSeparator(Renderer* renderer, int width) const;

protected:
    void RenderContent(Renderer* renderer, TranslationViewModel* model) override;

public:
    TranslationView(ViewContext* context, View* parentView, ModelHolder<TranslationViewModel*>* modelHolder, IComponent* component);

    void Initialize() override;

    Subscribable<> OnForceTranslation;
    Subscribable<> OnTranslateSuggestion;
};
