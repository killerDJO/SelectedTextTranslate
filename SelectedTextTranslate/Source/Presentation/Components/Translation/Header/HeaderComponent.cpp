#include "Presentation\Components\Translation\Header\HeaderComponent.h"

HeaderComponent::HeaderComponent(ServiceRegistry* serviceRegistry, View* parentView, ModelHolder<TranslationViewModel*>* modelHolder)
    : Component(new HeaderView(serviceRegistry->Get<ViewContext>(), parentView, modelHolder, this))
{
    this->textPlayer = serviceRegistry->Get<TextPlayer>();
    this->modelHolder = modelHolder;

    CurrentView->OnForceTranslation.Subscribe(&OnForceTranslation);
    CurrentView->OnPlayText.Subscribe(bind(&HeaderComponent::PlayText, this));
    CurrentView->OnTranslateSuggestion.Subscribe(&OnTranslateSuggestion);
}

void HeaderComponent::PlayText() const
{
    textPlayer->PlayText(modelHolder->GetModel()->GetTranslateResult().GetSentence().GetOrigin());
}