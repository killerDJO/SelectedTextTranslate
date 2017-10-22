#pragma once
#include "BusinessLogic\Translation\Dto\TranslateResult.h"
#include "Presentation\Framework\Rendering\Dto\RenderDescriptor.h"
#include "Presentation\Framework\Rendering\Dto\RenderResult.h"
#include "Presentation\Framework\Views\ComponentView.h"
#include "Presentation\Components\Translation\Content\TranslateResultViewModel.h"

class TranslateResultView : public ComponentView<TranslateResultViewModel>
{
private:
    RenderResult CreateExpandButton(TranslateResultViewModel model, RenderDescriptor renderDescriptor, TranslateResultCategory category, int categoryIndex, int showedCount);

protected:
    Size RenderContent(Renderer* renderer, TranslateResultViewModel model) override;

public:
    TranslateResultView(CommonContext* context, View* parentWindow, ModelHolder<TranslateResultViewModel>* modelHolder);
    Subscribeable<int> OnExpandCategory;
};
