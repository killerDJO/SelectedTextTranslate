#pragma once
#include "View\Content\Translation\Header\HeaderComponent.h"
#include "View\Content\Translation\Content\TranslateResultComponent.h"

class TranslationView : public ComponentView<TranslateResult>
{
private:
    int headerHeight;
    int separatorHeight;

    HeaderComponent* headerComponent;
    TranslateResultComponent* translateResultComponent;

    void RenderSeparator(Renderer* renderer, int width) const;

protected:
    Size RenderContent(Renderer* renderer, TranslateResult model) override;
    void Resize() override;

public:
    TranslationView(ViewContext* context, View* parentView, ModelHolder<TranslateResult>* modelHolder);

    void Initialize() override;

    Subscribeable<> OnForceTranslation;
    Subscribeable<> OnTranslateSuggestion;
};