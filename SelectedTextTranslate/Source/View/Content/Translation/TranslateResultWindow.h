#pragma once
#include "View\Content\Base\ContentWindow.h"
#include "Services\Translation\Dto\TranslateResult.h"
#include "Utilities\Subscribeable.h"

class TranslateResultWindow : public ContentWindow
{
private:
    HFONT fontUnderscored;

    TranslateResult translateResult;

    int CreateExpandButton(TranslateResultCategory category, int categoryIndex, int showedCount, int curY, Renderer* renderer);

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    TranslateResultWindow(WindowContext* context, Window* parentWindow);
    ~TranslateResultWindow() override;

    Subscribeable<int> OnExpandTranslationResult;

    void SetModel(TranslateResult translateResult);
};