#include "Windows\Content\HeaderWindow.h"

HeaderWindow::HeaderWindow(HINSTANCE hInstance, RenderingContext* renderingContext, ScrollProvider* scrollProvider, WindowDescriptor descriptor, HWND parentWindow, AppModel* appModel)
: ContentWindow(hInstance, renderingContext, scrollProvider, descriptor, parentWindow, appModel)
{
}

void HeaderWindow::PlayText()
{
    appModel->PlayCurrentText();
}

void HeaderWindow::Initialize()
{
    ContentWindow::Initialize();

    fontSmallUnderscored = renderingContext->CreateCustomFont(hWindow, FontSizes::Small, false, true);
}

SIZE HeaderWindow::RenderDC(Renderer* renderer)
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

    renderer->PrintText(headerText, fontHeader, Colors::Black, paddingX, curY);

    curY += lineHeight;

    DWORD imageSize = 15;
    HoverIconButtonWindow* audioButton = new HoverIconButtonWindow(
        hInstance,
        renderingContext,
        scrollProvider,
        renderingContext->Scale(WindowDescriptor::CreateFixedWindowDescriptor(paddingX, curY - smallFontAscent, imageSize, imageSize)),
        hWindow,
        IDB_AUDIO_INACTIVE,
        IDB_AUDIO,
        bind(&HeaderWindow::PlayText, this));

    AddChildWindow(audioButton);

    POINT originLintBottomRight = renderer->PrintText(
        subHeaderText,
        fontSmall,
        Colors::Gray,
        paddingX + imageSize + 2,
        curY);

    if (translateResult.IsInputCorrected())
    {
        PrintInputCorrectionWarning(translateResult.Sentence.Input, curY, originLintBottomRight, renderer);
    }

    renderer->IncreaseWidth(paddingX * 3);

    SIZE renderedSize = renderer->GetSize();
    int windowHeight = renderingContext->Downscale(descriptor.Height);
    int windowWidth = renderingContext->Downscale(descriptor.Width);
    RECT rect;
    rect.left = 0;
    rect.right = max(renderedSize.cx, windowWidth);
    rect.top = windowHeight - 1;
    rect.bottom = windowHeight;

    renderer->DrawRect(rect, grayBrush);

    return renderer->GetScaledSize();
}

void HeaderWindow::PrintInputCorrectionWarning(const wchar_t* originalInput, int curY, POINT originLintBottomRight, Renderer* renderer)
{
    originLintBottomRight = renderer->PrintText(
        L" (corrected from ",
        fontSmall,
        Colors::Gray,
        originLintBottomRight.x + 1,
        curY);

    int smallFontAscent = renderer->GetFontAscent(fontSmall);
    HoverTextButtonWindow* forceTranslationButton = new HoverTextButtonWindow(
        hInstance,
        renderingContext,
        scrollProvider,
        renderingContext->Scale(WindowDescriptor::CreateStretchWindowDescriptor(originLintBottomRight.x, curY - smallFontAscent)),
        hWindow,
        fontSmallUnderscored,
        Colors::Gray,
        Colors::Black,
        originalInput,
        bind(&HeaderWindow::PlayText, this));

    AddChildWindow(forceTranslationButton);

    originLintBottomRight = renderer->PrintText(
        L")",
        fontSmall,
        Colors::Gray,
        originLintBottomRight.x + renderingContext->Downscale(forceTranslationButton->GetWidth()),
        curY);
}

HeaderWindow::~HeaderWindow()
{
    DeleteObject(fontSmallUnderscored);
}