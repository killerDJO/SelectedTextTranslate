#pragma once
#include "BusinessLogic\Translation\Dto\TranslateResult.h"
#include "Utilities\Subscribeable.h"
#include "Presentation\Framework\Rendering\Dto\RenderDescriptor.h"
#include "Presentation\Framework\ModelHolder.h"
#include "Presentation\Framework\Views\ComponentView.h"
#include "Presentation\Components\Translation\TranslationViewModel.h"

class HeaderView : public ComponentView<TranslationViewModel*>
{
private:
    void PrintInputCorrectionWarning(RenderDescriptor renderDescriptor, wstring originalInput);
    void PrintSuggestion(RenderDescriptor renderDescriptor, wstring suggestion);
    void PrintHeaderAction(RenderDescriptor renderDescriptor, wstring actionDescription, wstring actionText, Subscribeable<>* actionCallback);

    Size RenderTranslationResult(Renderer* renderer, TranslateResult translateResult);
    Size RenderEmptyResult(Renderer* renderer) const;

protected:
    Size RenderContent(Renderer* renderer, TranslationViewModel* model) override;

public:
    HeaderView(CommonContext* context, View* parentView, ModelHolder<TranslationViewModel*>* modelHolder);

    Subscribeable<> OnPlayText;
    Subscribeable<> OnForceTranslation;
    Subscribeable<> OnTranslateSuggestion;
};
