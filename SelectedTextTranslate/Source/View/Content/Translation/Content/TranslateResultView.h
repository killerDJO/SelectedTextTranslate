#pragma once
#include "Services\Translation\Dto\TranslateResult.h"
#include "View\Framework\Rendering\Dto\RenderDescriptor.h"
#include "View\Framework\Rendering\Dto\RenderResult.h"
#include "View\Framework\View\Views\ComponentView.h"
#include "View\Content\Translation\Content\TranslateResultViewModel.h"

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
