#pragma once
#include "BusinessLogic\Translation\Dto\TranslateResult.h"
#include "Utilities\Subscribeable.h"
#include "Presentation\Framework\Rendering\Dto\RenderDescriptor.h"
#include "Presentation\Framework\ModelHolder.h"
#include "Presentation\Framework\Views\ComponentView.h"

class HeaderView : public ComponentView<TranslateResult>
{
private:
    void PrintInputCorrectionWarning(RenderDescriptor renderDescriptor, wstring originalInput);
    void PrintSuggestion(RenderDescriptor renderDescriptor, wstring suggestion);
    void PrintHeaderAction(RenderDescriptor renderDescriptor, wstring actionDescription, wstring actionText, Subscribeable<>* actionCallback);

    Size RenderTranslationResult(Renderer* renderer, TranslateResult model);
    Size RenderEmptyResult(Renderer* renderer) const;

protected:
    Size RenderContent(Renderer* renderer, TranslateResult model) override;

public:
    HeaderView(CommonContext* context, View* parentView, ModelHolder<TranslateResult>* modelHolder);

    Subscribeable<> OnPlayText;
    Subscribeable<> OnForceTranslation;
    Subscribeable<> OnTranslateSuggestion;
};