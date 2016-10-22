#include "View\Content\Translation\HeaderWindow.h"
#include "View\Controls\Buttons\HoverIconButtonWindow.h"
#include "View\Controls\Buttons\HoverTextButtonWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

HeaderWindow::HeaderWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow)
    : ContentWindow(context, descriptor, parentWindow)
{
    this->fontSmallUnderscored = nullptr;
    this->OnPlayText = Subscribeable<>();
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

    renderer->IncreaseWidth(paddingX);

    return renderer->GetScaledSize();
}

void HeaderWindow::PrintInputCorrectionWarning(const wchar_t* originalInput, int curY, Point originLintBottomRight, Renderer* renderer)
{
    originLintBottomRight = renderer->PrintText(
        L" (corrected from ",
        fontSmall,
        Colors::Gray,
        Point(originLintBottomRight.X + 1, curY));

    int smallFontAscent = renderer->GetFontAscent(fontSmall);
    HoverTextButtonWindow* forceTranslationButton = new HoverTextButtonWindow(
        context,
        WindowDescriptor::CreateStretchWindowDescriptor(Point(originLintBottomRight.X, curY - smallFontAscent)),
        this,
        fontSmallUnderscored,
        Colors::Gray,
        Colors::Black,
        originalInput);

    AddChildWindow(forceTranslationButton);

    renderer->PrintText(
        L")",
        fontSmall,
        Colors::Gray,
        Point(originLintBottomRight.X + context->GetScaleProvider()->Downscale(forceTranslationButton->GetSize().Width), curY));
}

HeaderWindow::~HeaderWindow()
{
    AssertCriticalWinApiResult(DeleteObject(fontSmallUnderscored));
}