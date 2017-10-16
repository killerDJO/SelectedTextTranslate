#pragma once
#include "Services\Translation\Dto\TranslateResult.h"
#include "View\Framework\ModelHolder.h"
#include "View\Content\Translation\Content\Dto\TranslateResultContent.h"
#include "View\Content\Translation\Content\TranslateResultView.h"

class TranslateResultComponent : public Component<TranslateResultView>, ModelHolder<TranslateResultContent>
{
private:
    TranslateResultContent contentModel;
    ModelHolder<TranslateResult>* modelHolder;

    void ExpandCategory(int categoryIndex);

public:
    TranslateResultComponent(ViewContext* context, View* parentView, ModelHolder<TranslateResult>* modelHolder);

    TranslateResultContent GetModel() override;
};