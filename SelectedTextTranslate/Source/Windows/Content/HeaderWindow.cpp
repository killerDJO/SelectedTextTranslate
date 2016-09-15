#include "Windows\Content\HeaderWindow.h"

HeaderWindow::HeaderWindow(Renderer* renderer, AppModel* appModel, HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD height)
: ContentWindow(renderer, appModel, parentWindow, hInstance, x, y, 0, height)
{
}

void HeaderWindow::PlayText()
{
    appModel->PlayCurrentText();
}

void HeaderWindow::Initialize()
{
    ContentWindow::Initialize();

    fontSmallUnderscored = renderer->CreateCustomFont(hWindow, FontSizes::Small, false, true);
}

POINT HeaderWindow::RenderDC()
{
    ContentWindow::RenderDC();

    TranslateResult translateResult = appModel->GetCurrentTranslateResult();

    DWORD imageSize = 15;
    DWORD adjustedSize = renderer->AdjustToYResolution(15);

    HoverIconButtonWindow* audioButton = new HoverIconButtonWindow(
        renderer,
        hWindow,
        hInstance,
        paddingX,
        paddingY / 2 + renderer->AdjustToYResolution(19) + (adjustedSize - imageSize) / 2,
        imageSize,
        imageSize,
        IDB_AUDIO_INACTIVE,
        IDB_AUDIO,
        bind(&HeaderWindow::PlayText, this));

    AddChildWindow(audioButton);

    POINT bottomRight = { 0, 0 };

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

    renderer->PrintText(inMemoryDC, headerText, fontHeader, Colors::Black, paddingX, curY, &bottomRight);

    int originLineY = curY + renderer->AdjustToYResolution(20);
    POINT originLintBottomRight = renderer->PrintText(
        inMemoryDC,
        subHeaderText,
        fontSmall,
        Colors::Gray,
        paddingX + renderer->AdjustToYResolution(16) - roundToInt((renderer->kY-1)*10),
        originLineY,
        &bottomRight);

    if (translateResult.IsInputCorrected())
    {
        PrintInputCorrectionWarning(translateResult.Sentence.Input, originLineY, originLintBottomRight, &bottomRight);
    }

    bottomRight.y = height;
    bottomRight.x += paddingX * 3;

    RECT rect;
    rect.left = 0;
    rect.right = bottomRight.x;
    rect.top = height - 1;
    rect.bottom = height;
    renderer->DrawRect(inMemoryDC, rect, grayBrush, &POINT());

    return bottomRight;
}

void HeaderWindow::PrintInputCorrectionWarning(const wchar_t* originalInput, int curY, POINT originLintBottomRight, POINT* bottomRight)
{
    originLintBottomRight = renderer->PrintText(
        inMemoryDC,
        L" (corrected from ",
        fontSmall,
        Colors::Gray,
        originLintBottomRight.x,
        curY,
        bottomRight);

    HoverTextButtonWindow* forceTranslationButton = new HoverTextButtonWindow(
        renderer,
        hWindow,
        hInstance,
        originLintBottomRight.x,
        curY,
        fontSmallUnderscored,
        Colors::Gray,
        Colors::Black,
        originalInput,
        bind(&HeaderWindow::PlayText, this));

    AddChildWindow(forceTranslationButton);

    originLintBottomRight = renderer->PrintText(
        inMemoryDC,
        L")",
        fontSmall,
        Colors::Gray,
        originLintBottomRight.x + forceTranslationButton->GetWidth(),
        curY,
        bottomRight);
}

HeaderWindow::~HeaderWindow()
{
    DeleteObject(fontSmallUnderscored);
}