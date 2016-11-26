#pragma once
#include "View\Content\Base\ContentWindow.h"
#include "Services\Translation\Dto\TranslateResult.h"
#include "Utilities\Subscribeable.h"
#include "View\Framework\ModelHolder.h"

class TranslateResultWindow : public ContentWindow, public ModelHolder<TranslateResult>
{
private:
    HFONT fontUnderscored;

    int CreateExpandButton(TranslateResultCategory category, int categoryIndex, int showedCount, int curY, Renderer* renderer);

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    TranslateResultWindow(WindowContext* context, Window* parentWindow);
    ~TranslateResultWindow() override;

    Subscribeable<int> OnExpandTranslationResult;
};
