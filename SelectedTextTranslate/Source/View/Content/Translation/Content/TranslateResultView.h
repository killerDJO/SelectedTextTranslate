#pragma once
#include "View\Framework\View\Views\ContentView.h"
#include "Services\Translation\Dto\TranslateResult.h"
#include "View\Framework\Rendering\Dto\RenderDescriptor.h"
#include "View\Framework\Rendering\Dto\RenderResult.h"
#include "View\Framework\View\Views\ComponentView.h"
#include "View\Content\Translation\Content\Dto\TranslateResultContent.h"

class TranslateResultView : public ComponentView<TranslateResultContent>
{
private:
    RenderResult CreateExpandButton(TranslateResultContent model, RenderDescriptor renderDescriptor, TranslateResultCategory category, int categoryIndex, int showedCount);

protected:
    Size RenderContent(Renderer* renderer, TranslateResultContent model) override;

public:
    TranslateResultView(ViewContext* context, View* parentWindow, ModelHolder<TranslateResultContent>* modelHolder);
    Subscribeable<int> OnExpandCategory;
};