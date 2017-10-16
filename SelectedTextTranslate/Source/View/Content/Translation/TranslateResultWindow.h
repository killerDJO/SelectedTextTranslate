#pragma once
#include "View\Framework\View\Views\ContentView.h"
#include "Services\Translation\Dto\TranslateResult.h"
#include "View\Framework\Rendering\Dto\RenderDescriptor.h"
#include "View\Framework\Rendering\Dto\RenderResult.h"
#include "View\Framework\ModelHolder.h"

class TranslateResultWindow : public ContentView, public ModelHolder<TranslateResult>
{
private:
    map<int, bool> translateResultCategoryExpandedMap;

    RenderResult CreateExpandButton(RenderDescriptor renderDescriptor, TranslateResultCategory category, int categoryIndex, int showedCount);

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    TranslateResultWindow(ViewContext* context, View* parentWindow);

    void SetModel(TranslateResult model) override;
};
