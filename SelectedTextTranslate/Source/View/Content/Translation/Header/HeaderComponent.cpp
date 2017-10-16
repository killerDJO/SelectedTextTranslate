#include "View\Content\Translation\Header\HeaderComponent.h"

HeaderComponent::HeaderComponent(ViewContext* context, View* parentView, ModelHolder<TranslateResult>* modelHolder, TextPlayer* textPlayer)
    : Component(new HeaderView(context, parentView, modelHolder))
{
    this->textPlayer = textPlayer;
    this->modelHolder = modelHolder;

    view->OnForceTranslation.Subscribe(&OnForceTranslation);
    view->OnPlayText.Subscribe(bind(&HeaderComponent::PlayText, this));
    view->OnTranslateSuggestion.Subscribe(&OnTranslateSuggestion);
}

void HeaderComponent::PlayText() const
{
    textPlayer->PlayText(modelHolder->GetModel().GetSentence().GetOrigin());
}