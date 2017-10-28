#include "Presentation\Components\Translation\Content\TranslateResultComponent.h"
#include "Presentation\Components\Translation\Content\TranslateResultView.h"

TranslateResultComponent::TranslateResultComponent(CommonContext* context, View* parentView, ModelHolder<TranslateResult>* modelHolder)
    : Component(context, new TranslateResultView(context, parentView, this))
{
    this->modelHolder = modelHolder;
    CurrentView->OnExpandCategory.Subscribe(bind(&TranslateResultComponent::ExpandCategory, this, placeholders::_1));
}

TranslateResultViewModel TranslateResultComponent::GetModel()
{
    TranslateResult model = modelHolder->GetModel();

    if (!model.EqualTo(viewModel.GetTranslateResult()))
    {
        viewModel = TranslateResultViewModel(model);
    }

    return viewModel;
}

void TranslateResultComponent::ExpandCategory(int categoryIndex)
{
    viewModel.ToggleCategory(categoryIndex);
    Render(true);
}