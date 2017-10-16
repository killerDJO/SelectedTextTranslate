#pragma once
#include "Services\Translation\Dto\TranslateResult.h"
#include "Utilities\Subscribeable.h"
#include "View\Framework\Rendering\Dto\RenderDescriptor.h"
#include "View\Framework\View\Views\ContentView.h"
#include "View\Framework\ModelHolder.h"

class HeaderWindow : public ContentView, public ModelHolder<TranslateResult>
{
private:
    void PrintInputCorrectionWarning(RenderDescriptor renderDescriptor, wstring originalInput);
    void PrintSuggestion(RenderDescriptor renderDescriptor, wstring suggestion);
    void PrintHeaderAction(RenderDescriptor renderDescriptor, wstring actionDescription, wstring actionText, Subscribeable<>* actionCallback);

    Size RenderTranslationResult(Renderer* renderer);
    Size RenderEmptyResult(Renderer* renderer) const;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    HeaderWindow(ViewContext* context, View* parentWindow);
    ~HeaderWindow() override;

    Subscribeable<> OnPlayText;
    Subscribeable<> OnForceTranslation;
    Subscribeable<> OnTranslateSuggestion;
};
