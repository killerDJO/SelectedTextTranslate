#pragma once
#include "BusinessLogic\Translation\Dto\TranslateResult.h"
#include "Utilities\Subscribable\Subscribable.h"
#include "Presentation\Framework\Rendering\Dto\RenderDescriptor.h"
#include "Presentation\Framework\ModelHolder.h"
#include "Presentation\Framework\Views\ChildComponentView.h"
#include "Presentation\Components\Translation\TranslationViewModel.h"

class HeaderView : public ChildComponentView<TranslationViewModel*>
{
private:
    void PrintInputCorrectionWarning(RenderDescriptor renderDescriptor, wstring originalInput);
    void PrintSuggestion(RenderDescriptor renderDescriptor, wstring suggestion);
    void PrintHeaderAction(RenderDescriptor renderDescriptor, wstring actionDescription, wstring actionText, Subscribable<>* actionCallback);

    Size RenderTranslationResult(Renderer* renderer, TranslateResult translateResult);
    Size RenderEmptyResult(Renderer* renderer) const;

protected:
    Size RenderContent(Renderer* renderer, TranslationViewModel* model) override;

public:
    HeaderView(ViewContext* context, View* parentView, ModelHolder<TranslationViewModel*>* modelHolder, IComponent* component);

    Subscribable<> OnPlayText;
    Subscribable<> OnForceTranslation;
    Subscribable<> OnTranslateSuggestion;
};
