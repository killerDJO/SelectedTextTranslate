#pragma once
#include "BusinessLogic\Translation\Dto\TranslateResult.h"
#include "Presentation\Framework\Rendering\Dto\RenderDescriptor.h"
#include "Presentation\Framework\Rendering\Dto\RenderResult.h"
#include "Presentation\Framework\Views\ChildComponentView.h"
#include "Presentation\Components\Translation\TranslationViewModel.h"

class TranslateResultView : public ChildComponentView<TranslationViewModel*>
{
private:
    RenderResult CreateExpandButton(TranslationViewModel* model, RenderDescriptor renderDescriptor, TranslateResultCategory category, int categoryIndex, int showedCount);

protected:
    Size RenderContent(Renderer* renderer, TranslationViewModel* model) override;

public:
    TranslateResultView(ViewContext* context, View* parentWindow, ModelHolder<TranslationViewModel*>* modelHolder, IComponent* component);
    Subscribable<int> OnExpandCategory;
};
