#pragma once
#include "BusinessLogic\Translation\Dto\TranslateResult.h"
#include "Presentation\Framework\ModelHolder.h"
#include "Presentation\Components\Translation\Content\TranslateResultViewModel.h"
#include "Presentation\Components\Translation\Content\TranslateResultView.h"

class TranslateResultComponent : public Component<TranslateResultView>, ModelHolder<TranslateResultViewModel>
{
private:
    TranslateResultViewModel contentModel;
    ModelHolder<TranslateResult>* modelHolder;

    void ExpandCategory(int categoryIndex);

public:
    TranslateResultComponent(CommonContext* context, View* parentView, ModelHolder<TranslateResult>* modelHolder);

    TranslateResultViewModel GetModel() override;
};