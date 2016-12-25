#pragma once
#include "View\Framework\Windows\ContentWindow.h"
#include "View\Content\Translation\HeaderWindow.h"
#include "View\Content\Translation\TranslateResultWindow.h"

class TranslationWindow : public ContentWindow, public ModelHolder<TranslateResult>
{
private:
    int headerHeight;
    int separatorHeight;

    HeaderWindow* headerWindow;
    TranslateResultWindow* translateResultWindow;

    void RenderSeparator(Renderer* renderer, int width) const;

protected:
    Size RenderContent(Renderer* renderer) override;
    void Resize() override;

public:
    TranslationWindow(WindowContext* context, Window* parentWindow);

    void Initialize() override;

    Subscribeable<> OnPlayText;
    Subscribeable<> OnForceTranslation;
    Subscribeable<> OnTranslateSuggestion;

    void SetModel(TranslateResult translateResult) override;
};