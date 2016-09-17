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

    int curY = paddingY / 2;

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
        renderingContext->Scale(WindowDescriptor::CreateFixedWindowDescriptor(paddingX, curY, imageSize, imageSize)),
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
    RECT rect;
    rect.left = 0;
    rect.right = max(renderedSize.cx, descriptor.Width);
    rect.top = descriptor.Height - 1;
    rect.bottom = descriptor.Height;

    renderer->DrawRect(rect, grayBrush);

    return renderer->GetScaledSize();
}

void HeaderWindow::PrintInputCorrectionWarning(const wchar_t* originalInput, int curY, POINT originLintBottomRight, Renderer* renderer)
{
    originLintBottomRight = renderer->PrintText(
        L" (corrected from ",
        fontSmall,
        Colors::Gray,
        originLintBottomRight.x,
        curY);

    HoverTextButtonWindow* forceTranslationButton = new HoverTextButtonWindow(
        hInstance,
        renderingContext,
        scrollProvider,
        renderingContext->Scale(WindowDescriptor::CreateStretchWindowDescriptor(originLintBottomRight.x, curY)),
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
        originLintBottomRight.x + forceTranslationButton->GetWidth(),
        curY);
}

HeaderWindow::~HeaderWindow()
{
    DeleteObject(fontSmallUnderscored);
}