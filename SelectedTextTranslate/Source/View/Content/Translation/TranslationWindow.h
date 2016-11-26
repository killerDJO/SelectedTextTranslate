#pragma once
#include "View\Content\Base\ContentWindow.h"
#include "View\Content\Translation\HeaderWindow.h"
#include "View\Content\Translation\TranslateResultWindow.h"

class TranslationWindow : public ContentWindow, public ModelHolder<TranslateResult>
{
private:
    const int headerHeight = 50;
    HBRUSH separatorBrush;
    HBRUSH disabledBackgroundBrush;

    HeaderWindow* headerWindow;
    TranslateResultWindow* translateResultWindow;

    void RenderSeparator(Renderer* renderer, int width) const;

protected:
    Size RenderContent(Renderer* renderer) override;
    void Resize() override;

public:
    TranslationWindow(WindowContext* context, Window* parentWindow);
    ~TranslationWindow() override;

    void Initialize() override;

    Subscribeable<> OnPlayText;
    Subscribeable<> OnForceTranslation;
    Subscribeable<> OnTranslateSuggestion;
    Subscribeable<int> OnExpandTranslationResult;

    void SetModel(TranslateResult translateResult) override;
};