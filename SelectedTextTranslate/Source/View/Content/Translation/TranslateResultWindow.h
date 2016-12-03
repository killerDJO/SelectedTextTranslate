#pragma once
#include "View\Content\Base\ContentWindow.h"
#include "Services\Translation\Dto\TranslateResult.h"
#include "Utilities\Subscribeable.h"
#include "View\Framework\ModelHolder.h"
#include "View\Framework\Dto\Rendering\RenderDescriptor.h"
#include "View\Framework\Dto\Rendering\RenderResult.h"

class TranslateResultWindow : public ContentWindow, public ModelHolder<TranslateResult>
{
private:
    RenderResult CreateExpandButton(RenderDescriptor renderDescriptor, TranslateResultCategory category, int categoryIndex, int showedCount);

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    TranslateResultWindow(WindowContext* context, Window* parentWindow);
    ~TranslateResultWindow() override;

    Subscribeable<int> OnExpandTranslationResult;
};
