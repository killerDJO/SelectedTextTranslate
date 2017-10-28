#include "Presentation\Components\Translation\Content\TranslateResultComponent.h"
#include "Presentation\Components\Translation\Content\TranslateResultView.h"

TranslateResultComponent::TranslateResultComponent(CommonContext* context, View* parentView, ModelHolder<TranslationViewModel*>* modelHolder)
    : Component(context, new TranslateResultView(context, parentView, modelHolder))
{
    this->modelHolder = modelHolder;
    CurrentView->OnExpandCategory.Subscribe(bind(&TranslateResultComponent::ExpandCategory, this, placeholders::_1));
}

void TranslateResultComponent::ExpandCategory(int categoryIndex) const
{
    modelHolder->GetModel()->ToggleCategory(categoryIndex);
    Render(true);
}
