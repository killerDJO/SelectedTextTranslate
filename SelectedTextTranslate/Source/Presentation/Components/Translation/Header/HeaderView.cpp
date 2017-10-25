#include "Presentation\Components\Translation\Header\HeaderView.h"
#include "Utilities\StringUtilities.h"
#include "Presentation\Controls\Buttons\HoverIconButtonControl.h"
#include "Presentation\Controls\Buttons\HoverTextButtonControl.h"

HeaderView::HeaderView(CommonContext* context, View* parentWindow, ModelHolder<TranslateResult>* modelHolder)
    : ComponentView<TranslateResult>(context, parentWindow, modelHolder)
{
    this->viewName = L"TranlsateResultHeaderWindow";
}

Size HeaderView::RenderContent(Renderer* renderer, TranslateResult model)
{
    DestroyChildViews();

    if(!model.IsEmptyResult())
    {
        return RenderTranslationResult(renderer, model);
    }
    else
    {
        return RenderEmptyResult(renderer);
    }
}

Size HeaderView::RenderTranslationResult(Renderer* renderer, TranslateResult model)
{
    RenderPosition renderPosition = RenderPosition(paddingX, lineHeight);

    TranslateResultSentence sentence = model.GetSentence();
    renderer->PrintText(sentence.GetTranslation(), fontHeader, Colors::Black, renderPosition);

    renderPosition = renderPosition.MoveY(lineHeight);

    int imageSize = fontSmall->GetAscent();
    HoverIconButtonControl* audioButton = new HoverIconButtonControl(context, this);
    audioButton->SetDimensions(
        renderPosition.MoveY(-imageSize).MoveY(2, scaleProvider).GetPosition(),
        Size(imageSize, imageSize));
    audioButton->SetHoverIconResource(IDR_AUDIO);
    audioButton->SetNormalIconResource(IDR_AUDIO_INACTIVE);
    audioButton->OnClick.Subscribe(&OnPlayText);
    audioButton->InitializeAndRender();

    renderPosition = renderPosition.MoveX(imageSize).MoveX(2, scaleProvider);
    renderPosition = renderer->PrintText(sentence.GetOrigin(), fontSmall, Colors::Gray, renderPosition);

    RenderDescriptor actionRenderDescriptor = RenderDescriptor(renderer, renderPosition.MoveX(1));
    if (model.IsInputCorrected())
    {
        PrintInputCorrectionWarning(actionRenderDescriptor, sentence.GetInput());
    }
    else if (!sentence.GetSuggestion().empty())
    {
        PrintSuggestion(actionRenderDescriptor, sentence.GetSuggestion());
    }

    renderer->IncreaseWidth(paddingX);

    return renderer->GetSize();
}

Size HeaderView::RenderEmptyResult(Renderer* renderer) const
{
    int curY = GetSize().GetHeight() / 2 + fontHeader->GetHeight() / 2;

    renderer->PrintText(wstring(L"No text data selected"), fontHeader, Colors::Gray, RenderPosition(paddingX, curY));

    return renderer->GetSize();
}

void HeaderView::PrintInputCorrectionWarning(RenderDescriptor renderDescriptor, wstring originalInput)
{
    PrintHeaderAction(renderDescriptor, L"corrected from", originalInput, &OnForceTranslation);
}

void HeaderView::PrintSuggestion(RenderDescriptor renderDescriptor, wstring suggestion)
{
    PrintHeaderAction(renderDescriptor, L"maybe you meant", suggestion, &OnTranslateSuggestion);
}

void HeaderView::PrintHeaderAction(RenderDescriptor renderDescriptor, wstring actionDescription, wstring actionText, Subscribeable<>* actionCallback)
{
    TextRenderResult originLineRenderResult = renderDescriptor.GetRenderer()->PrintText(
        StringUtilities::Format(L" (%ls ", actionDescription.c_str()),
        fontSmall,
        Colors::Gray,
        renderDescriptor.GetRenderPosition());

    HoverTextButtonControl* headerActionButton = new HoverTextButtonControl(context, this);
    headerActionButton->SetFont(fontSmallUnderscored);
    headerActionButton->SetPosition(originLineRenderResult.GetRenderPosition().MoveY(-fontSmall->GetAscent()).GetPosition());
    headerActionButton->SetText(actionText);
    headerActionButton->OnClick.Subscribe(actionCallback);
    headerActionButton->InitializeAndRender();

    renderDescriptor.GetRenderer()->PrintText(
        L")",
        fontSmall,
        Colors::Gray,
        Point(headerActionButton->GetBoundingRect().GetRight(), originLineRenderResult.GetBaselineY()));
}