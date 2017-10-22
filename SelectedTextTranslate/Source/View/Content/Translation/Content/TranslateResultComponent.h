#pragma once
#include "Services\Translation\Dto\TranslateResult.h"
#include "View\Framework\ModelHolder.h"
#include "View\Content\Translation\Content\TranslateResultViewModel.h"
#include "View\Content\Translation\Content\TranslateResultView.h"

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