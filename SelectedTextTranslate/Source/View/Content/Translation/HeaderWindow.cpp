#include "View\Content\Translation\HeaderWindow.h"
#include "View\Controls\Buttons\HoverIconButtonWindow.h"
#include "View\Controls\Buttons\HoverTextButtonWindow.h"

HeaderWindow::HeaderWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow, AppModel* appModel)
: ContentWindow(context, descriptor, parentWindow, appModel)
{
    fontSmallUnderscored = nullptr;
}

void HeaderWindow::PlayText() const
{
    appModel->PlayCurrentText();
}

void HeaderWindow::Initialize()
{
    ContentWindow::Initialize();

    fontSmallUnderscored = context->GetRenderingContext()->CreateCustomFont(windowHandle, FontSizes::Small, false, true);
}

Size HeaderWindow::RenderContent(Renderer* renderer)
{
    ContentWindow::RenderContent(renderer);
    DestroyChildWindows();

    TranslateResult translateResult = appModel->GetCurrentTranslateResult();

    int smallFontAscent = renderer->GetFontAscent(fontSmall);
    int curY = lineHeight;

    const wchar_t* headerText;
    const wchar_t* subHeaderText;

    if (translateResult.IsErrorResult())
    {
        headerText = L"[Error translating sentence]";
        subHeaderText = translateResult.ErrorMessage;
    }
    else
    {
        headerText = translateResult.Sentence.Translation;
        subHeaderText = translateResult.Sentence.Origin;
    }

    renderer->PrintText(headerText, fontHeader, Colors::Black, Point(paddingX, curY));

    curY += lineHeight;

    int imageSize = smallFontAscent;
    HoverIconButtonWindow* audioButton = new HoverIconButtonWindow(
        context,
        WindowDescriptor::CreateFixedWindowDescriptor(Point(paddingX, curY - imageSize + 2), Size(imageSize, imageSize)),
        this,
        IDR_AUDIO_INACTIVE,
        IDR_AUDIO,
        bind(&HeaderWindow::PlayText, this));

    AddChildWindow(audioButton);

    Point originLintBottomRight = renderer->PrintText(
        subHeaderText,
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
        originalInput,
        bind(&HeaderWindow::PlayText, this));

    AddChildWindow(forceTranslationButton);

    renderer->PrintText(
        L")",
        fontSmall,
        Colors::Gray,
        Point(originLintBottomRight.X + context->GetScaleProvider()->Downscale(forceTranslationButton->GetSize().Width), curY));
}

HeaderWindow::~HeaderWindow()
{
    DeleteObject(fontSmallUnderscored);
}