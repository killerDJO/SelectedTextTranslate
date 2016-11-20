#pragma once
#include "View\Content\Base\ContentWindow.h"
#include "Services\Translation\Dto\TranslateResult.h"
#include "Utilities\Subscribeable.h"

class HeaderWindow : public ContentWindow
{
private:
    HFONT fontSmallUnderscored;

    TranslateResult translateResult;

    void PrintInputCorrectionWarning(wstring originalInput, int curY, Point originLineBottomRight, Renderer* renderer);
    void PrintSuggestion(wstring suggestion, int curY, Point originLineBottomRight, Renderer* renderer);
    void PrintHeaderAction(wstring actionDescription, wstring actionText, Subscribeable<>* actionCallback, int curY, Point originLineBottomRight, Renderer* renderer);

    Size RenderTranslationResult(Renderer* renderer);
    Size RenderEmptyResult(Renderer* renderer) const;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    HeaderWindow(WindowContext* context, Window* parentWindow);
    ~HeaderWindow() override;

    Subscribeable<> OnPlayText;
    Subscribeable<> OnForceTranslation;
    Subscribeable<> OnTranslateSuggestion;

    void SetModel(TranslateResult translateResult);
};
