#pragma once
#include "View\Framework\View\Views\ContentView.h"
#include "View\Content\Translation\HeaderWindow.h"
#include "View\Content\Translation\TranslateResultWindow.h"

class TranslationWindow : public ContentView, public ModelHolder<TranslateResult>
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
    TranslationWindow(ViewContext* context, View* parentWindow);

    void Initialize() override;

    Subscribeable<> OnPlayText;
    Subscribeable<> OnForceTranslation;
    Subscribeable<> OnTranslateSuggestion;

    void SetModel(TranslateResult translateResult) override;
};