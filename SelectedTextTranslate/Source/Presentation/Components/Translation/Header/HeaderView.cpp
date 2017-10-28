#include "Presentation\Components\Translation\Header\HeaderView.h"
#include "Utilities\StringUtilities.h"
#include "Presentation\Controls\Buttons\HoverIconButtonControl.h"
#include "Presentation\Controls\Buttons\HoverTextButtonControl.h"

HeaderView::HeaderView(CommonContext* context, View* parentWindow, ModelHolder<TranslateResult>* modelHolder)
    : ComponentView<TranslateResult>(context, parentWindow, modelHolder)
{
    Name = L"TranlsateResultHeaderWindow";
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
    RenderPosition renderPosition = RenderPosition(PaddingX, LineHeight);

    TranslateResultSentence sentence = model.GetSentence();
    renderer->PrintText(sentence.GetTranslation(), FontHeader, Colors::Black, renderPosition);

    renderPosition = renderPosition.MoveY(LineHeight);

    int imageSize = FontSmall->GetAscent();
    HoverIconButtonControl* audioButton = new HoverIconButtonControl(Context, this);
    audioButton->SetDimensions(
        renderPosition.MoveY(-imageSize).MoveY(2, ScaleProvider).GetPosition(),
        Size(imageSize, imageSize));
    audioButton->SetHoverIconResource(IDR_AUDIO);
    audioButton->SetNormalIconResource(IDR_AUDIO_INACTIVE);
    audioButton->OnClick.Subscribe(&OnPlayText);
    audioButton->InitializeAndRender();

    renderPosition = renderPosition.MoveX(imageSize).MoveX(2, ScaleProvider);
    renderPosition = renderer->PrintText(sentence.GetOrigin(), FontSmall, Colors::Gray, renderPosition);

    RenderDescriptor actionRenderDescriptor = RenderDescriptor(renderer, renderPosition.MoveX(1));
    if (model.IsInputCorrected())
    {
        PrintInputCorrectionWarning(actionRenderDescriptor, sentence.GetInput());
    }
    else if (!sentence.GetSuggestion().empty())
    {
        PrintSuggestion(actionRenderDescriptor, sentence.GetSuggestion());
    }

    renderer->IncreaseWidth(PaddingX);

    return renderer->GetSize();
}

Size HeaderView::RenderEmptyResult(Renderer* renderer) const
{
    int curY = GetSize().GetHeight() / 2 + FontHeader->GetHeight() / 2;

    renderer->PrintText(wstring(L"No text data selected"), FontHeader, Colors::Gray, RenderPosition(PaddingX, curY));

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
        FontSmall,
        Colors::Gray,
        renderDescriptor.GetRenderPosition());

    HoverTextButtonControl* headerActionButton = new HoverTextButtonControl(Context, this);
    headerActionButton->SetFont(FontSmallUnderscored);
    headerActionButton->SetPosition(originLineRenderResult.GetRenderPosition().MoveY(-FontSmall->GetAscent()).GetPosition());
    headerActionButton->SetText(actionText);
    headerActionButton->OnClick.Subscribe(actionCallback);
    headerActionButton->InitializeAndRender();

    renderDescriptor.GetRenderer()->PrintText(
        L")",
        FontSmall,
        Colors::Gray,
        Point(headerActionButton->GetBoundingRect().GetRight(), originLineRenderResult.GetBaselineY()));
}