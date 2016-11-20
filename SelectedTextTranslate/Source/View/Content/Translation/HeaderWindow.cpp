#include "View\Content\Translation\HeaderWindow.h"
#include "View\Controls\Buttons\HoverIconButtonWindow.h"
#include "View\Controls\Buttons\HoverTextButtonWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Utilities\StringUtilities.h"

HeaderWindow::HeaderWindow(WindowContext* context, Window* parentWindow)
    : ContentWindow(context, parentWindow)
{
    this->OnPlayText = Subscribeable<>();
    this->OnForceTranslation = Subscribeable<>();
    this->OnTranslateSuggestion = Subscribeable<>();
    this->fontSmallUnderscored = context->GetRenderingContext()->CreateCustomFont(FontSizes::Small, false, true);
}

void HeaderWindow::SetModel(TranslateResult translateResult)
{
    AssertWindowInitialized();

    this->translateResult = translateResult;
}

Size HeaderWindow::RenderContent(Renderer* renderer)
{
    ContentWindow::RenderContent(renderer);
    DestroyChildWindows();

    if(!translateResult.IsEmptyResult())
    {
        return RenderTranslationResult(renderer);
    }
    else
    {
        return RenderEmptyResult(renderer);
    }
}

Size HeaderWindow::RenderTranslationResult(Renderer* renderer)
{
    int smallFontAscent = renderer->GetFontAscent(fontSmall);
    int curY = lineHeight;

    TranslateResultSentence sentence = translateResult.GetSentence();
    renderer->PrintText(sentence.GetTranslation(), fontHeader, Colors::Black, Point(paddingX, curY));

    curY += lineHeight;

    int imageSize = smallFontAscent;
    HoverIconButtonWindow* audioButton = new HoverIconButtonWindow(context, this);
    audioButton->SetDimensions(Point(paddingX, curY - imageSize + 2), Size(imageSize, imageSize));
    audioButton->SetHoverIconResource(IDR_AUDIO);
    audioButton->SetNormalIconResource(IDR_AUDIO_INACTIVE);
    audioButton->OnClick.Subscribe(&OnPlayText);

    AddChildWindow(audioButton);

    Point originLintBottomRight = renderer->PrintText(
        sentence.GetOrigin(),
        fontSmall,
        Colors::Gray,
        Point(paddingX + imageSize + 2, curY));

    if (translateResult.IsInputCorrected())
    {
        PrintInputCorrectionWarning(sentence.GetInput(), curY, originLintBottomRight, renderer);
    }
    else if (!sentence.GetSuggestion().empty())
    {
        PrintSuggestion(sentence.GetSuggestion(), curY, originLintBottomRight, renderer);
    }

    renderer->IncreaseWidth(paddingX);

    return renderer->GetScaledSize();
}

Size HeaderWindow::RenderEmptyResult(Renderer* renderer) const
{
    int fontHeight = renderer->GetFontStrokeHeight(fontHeader);
    int curY = context->GetScaleProvider()->Downscale(windowSize.Height / 2) + fontHeight / 2;

    renderer->PrintText(wstring(L"No text data selected"), fontHeader, Colors::Gray, Point(paddingX, curY));

    return renderer->GetScaledSize();
}

void HeaderWindow::PrintInputCorrectionWarning(wstring originalInput, int curY, Point originLintBottomRight, Renderer* renderer)
{
    PrintHeaderAction(L"corrected from", originalInput, &OnForceTranslation, curY, originLintBottomRight, renderer);
}

void HeaderWindow::PrintSuggestion(wstring suggestion, int curY, Point originLintBottomRight, Renderer* renderer)
{
    PrintHeaderAction(L"maybe you meant", suggestion, &OnTranslateSuggestion, curY, originLintBottomRight, renderer);
}

void HeaderWindow::PrintHeaderAction(wstring actionDescription, wstring actionText, Subscribeable<>* actionCallback, int curY, Point originLineBottomRight, Renderer* renderer)
{
    originLineBottomRight = renderer->PrintText(
        StringUtilities::Format(L" (%ls ", actionDescription.c_str()),
        fontSmall,
        Colors::Gray,
        Point(originLineBottomRight.X + 1, curY));

    int smallFontAscent = renderer->GetFontAscent(fontSmall);
    HoverTextButtonWindow* forceTranslationButton = new HoverTextButtonWindow(context, this);
    forceTranslationButton->SetFont(fontSmallUnderscored);
    forceTranslationButton->SetPosition(Point(originLineBottomRight.X, curY - smallFontAscent));
    forceTranslationButton->SetText(actionText);
    forceTranslationButton->OnClick.Subscribe(actionCallback);

    AddChildWindow(forceTranslationButton);

    renderer->PrintText(
        L")",
        fontSmall,
        Colors::Gray,
        Point(originLineBottomRight.X + context->GetScaleProvider()->Downscale(forceTranslationButton->GetSize().Width), curY));
}

HeaderWindow::~HeaderWindow()
{
    AssertCriticalWinApiResult(DeleteObject(fontSmallUnderscored));
}
