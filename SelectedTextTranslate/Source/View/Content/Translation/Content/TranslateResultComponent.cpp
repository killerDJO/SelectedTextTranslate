#include "View\Content\Translation\Content\TranslateResultComponent.h"
#include "View\Content\Translation\Content\TranslateResultView.h"

TranslateResultComponent::TranslateResultComponent(CommonContext* context, View* parentView, ModelHolder<TranslateResult>* modelHolder)
    : Component(context, new TranslateResultView(context, parentView, this))
{
    this->modelHolder = modelHolder;
    view->OnExpandCategory.Subscribe(bind(&TranslateResultComponent::ExpandCategory, this, placeholders::_1));
}

TranslateResultViewModel TranslateResultComponent::GetModel()
{
    TranslateResult model = modelHolder->GetModel();

    if (!model.EqualTo(contentModel.GetTranslateResult()))
    {
        contentModel = TranslateResultViewModel(model);
    }

    return contentModel;
}

void TranslateResultComponent::ExpandCategory(int categoryIndex)
{
    contentModel.ToggleCategory(categoryIndex);
    Render(true);
}