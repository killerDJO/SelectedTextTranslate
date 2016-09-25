#include "Windows\Content\HeaderWindow.h"
#include "Windows\Buttons\HoverIconButtonWindow.h"
#include "Windows\Buttons\HoverTextButtonWindow.h"

HeaderWindow::HeaderWindow(HINSTANCE hInstance, RenderingContext* renderingContext, ScrollProvider* scrollProvider, WindowDescriptor descriptor, HWND parentWindow, AppModel* appModel)
: ContentWindow(hInstance, renderingContext, scrollProvider, descriptor, parentWindow, appModel)
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

    fontSmallUnderscored = renderingContext->CreateCustomFont(hWindow, FontSizes::Small, false, true);
}

Size HeaderWindow::RenderDC(Renderer* renderer)
{
    ContentWindow::RenderDC(renderer);

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
        hInstance,
        renderingContext,
        scrollProvider,
        WindowDescriptor::CreateFixedWindowDescriptor(Point(paddingX, curY - imageSize + 2), Size(imageSize, imageSize)),
        hWindow,
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

    renderer->IncreaseWidth(paddingX * 3);

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
        hInstance,
        renderingContext,
        scrollProvider,
        WindowDescriptor::CreateStretchWindowDescriptor(Point(originLintBottomRight.X, curY - smallFontAscent)),
        hWindow,
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
        Point(originLintBottomRight.X + renderingContext->Downscale(forceTranslationButton->GetWidth()), curY));
}

HeaderWindow::~HeaderWindow()
{
    DeleteObject(fontSmallUnderscored);
}