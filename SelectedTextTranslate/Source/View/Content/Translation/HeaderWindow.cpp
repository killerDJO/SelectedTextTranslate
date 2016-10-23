#include "View\Content\Translation\HeaderWindow.h"
#include "View\Controls\Buttons\HoverIconButtonWindow.h"
#include "View\Controls\Buttons\HoverTextButtonWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Utilities\StringUtilities.h"

HeaderWindow::HeaderWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow)
    : ContentWindow(context, descriptor, parentWindow)
{
    this->fontSmallUnderscored = nullptr;
    this->OnPlayText = Subscribeable<>();
    this->OnForceTranslation = Subscribeable<>();
    this->OnTranslateSuggestion = Subscribeable<>();
}

void HeaderWindow::Initialize()
{
    ContentWindow::Initialize();

    fontSmallUnderscored = context->GetRenderingContext()->CreateCustomFont(windowHandle, FontSizes::Small, false, true);
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

    int smallFontAscent = renderer->GetFontAscent(fontSmall);
    int curY = lineHeight;

    renderer->PrintText(translateResult.Sentence.Translation, fontHeader, Colors::Black, Point(paddingX, curY));

    curY += lineHeight;

    int imageSize = smallFontAscent;
    HoverIconButtonWindow* audioButton = new HoverIconButtonWindow(
        context,
        WindowDescriptor::CreateFixedWindowDescriptor(Point(paddingX, curY - imageSize + 2), Size(imageSize, imageSize)),
        this,
        IDR_AUDIO_INACTIVE,
        IDR_AUDIO);
    audioButton->OnClick.Subscribe(&OnPlayText);

    AddChildWindow(audioButton);

    Point originLintBottomRight = renderer->PrintText(
        translateResult.Sentence.Origin,
        fontSmall,
        Colors::Gray,
        Point(paddingX + imageSize + 2, curY));

    if (translateResult.IsInputCorrected())
    {
        PrintInputCorrectionWarning(translateResult.Sentence.Input, curY, originLintBottomRight, renderer);
    }
    else if(!translateResult.Sentence.Suggestion.empty())
    {
        PrintSuggestion(translateResult.Sentence.Suggestion, curY, originLintBottomRight, renderer);
    }

    renderer->IncreaseWidth(paddingX);

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
    HoverTextButtonWindow* forceTranslationButton = new HoverTextButtonWindow(
        context,
        WindowDescriptor::CreateStretchWindowDescriptor(Point(originLineBottomRight.X, curY - smallFontAscent)),
        this,
        fontSmallUnderscored,
        Colors::Gray,
        Colors::Black,
        actionText);
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
