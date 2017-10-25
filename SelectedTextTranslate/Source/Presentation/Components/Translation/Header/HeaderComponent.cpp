#include "Presentation\Components\Translation\Header\HeaderComponent.h"

HeaderComponent::HeaderComponent(CommonContext* context, View* parentView, ModelHolder<TranslateResult>* modelHolder)
    : Component(context, new HeaderView(context, parentView, modelHolder))
{
    this->textPlayer = context->Get<TextPlayer>();
    this->modelHolder = modelHolder;

    CurrentView->OnForceTranslation.Subscribe(&OnForceTranslation);
    CurrentView->OnPlayText.Subscribe(bind(&HeaderComponent::PlayText, this));
    CurrentView->OnTranslateSuggestion.Subscribe(&OnTranslateSuggestion);
}

void HeaderComponent::PlayText() const
{
    textPlayer->PlayText(modelHolder->GetModel().GetSentence().GetOrigin());
}