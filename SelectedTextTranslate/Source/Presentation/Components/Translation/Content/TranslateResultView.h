#pragma once
#include "BusinessLogic\Translation\Dto\TranslateResult.h"
#include "Presentation\Framework\Rendering\Dto\RenderDescriptor.h"
#include "Presentation\Framework\Rendering\Dto\RenderResult.h"
#include "Presentation\Framework\Views\ComponentView.h"
#include "Presentation\Components\Translation\TranslationViewModel.h"

class TranslateResultView : public ComponentView<TranslationViewModel*>
{
private:
    RenderResult CreateExpandButton(TranslationViewModel* model, RenderDescriptor renderDescriptor, TranslateResultCategory category, int categoryIndex, int showedCount);

protected:
    Size RenderContent(Renderer* renderer, TranslationViewModel* model) override;

public:
    TranslateResultView(CommonContext* context, View* parentWindow, ModelHolder<TranslationViewModel*>* modelHolder);
    Subscribeable<int> OnExpandCategory;
};
