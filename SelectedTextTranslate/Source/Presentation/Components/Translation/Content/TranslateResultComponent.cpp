#include "Presentation\Components\Translation\Content\TranslateResultComponent.h"
#include "Presentation\Components\Translation\Content\TranslateResultView.h"

TranslateResultComponent::TranslateResultComponent(ServiceRegistry* serviceRegistry, View* parentView, ModelHolder<TranslationViewModel*>* modelHolder)
    : Component(new TranslateResultView(serviceRegistry->Get<ViewContext>(), parentView, modelHolder, this))
{
    this->modelHolder = modelHolder;
    CurrentView->OnExpandCategory.Subscribe(bind(&TranslateResultComponent::ExpandCategory, this, placeholders::_1));
}

void TranslateResultComponent::ExpandCategory(int categoryIndex) const
{
    modelHolder->GetModel()->ToggleCategory(categoryIndex);
    Render(true);
}
