#include "Windows\Content\HeaderWindow.h"

HeaderWindow::HeaderWindow(AppModel* appModel, HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height)
: ContentWindow(appModel, parentWindow, hInstance, x, y, width, height) 
{
}

void HeaderWindow::PlayText()
{
    this->appModel->PlayCurrentText();
}

void HeaderWindow::InitializeFonts()
{
    ContentWindow::InitializeFonts();

    HDC hdc = GetDC(hWindow);

    long lfHeightSmall = -MulDiv(int(fontHeight * 3 / 7.0), GetDeviceCaps(hdc, LOGPIXELSY), 72);
    this->fontSmallUnderscored = CreateFont(lfHeightSmall, 0, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
}

POINT HeaderWindow::RenderDC()
{
    ContentWindow::RenderDC();

    TranslateResult translateResult = this->appModel->GetCurrentTranslateResult();

    DWORD imageSize = 15;
    DWORD adjustedSize = AdjustToResolution(15, kY);

    HoverIconButtonWindow* audioButton = new HoverIconButtonWindow(
        this->hWindow,
        this->hInstance,
        paddingX,
        paddingY / 2 + AdjustToResolution(19, kY) + (adjustedSize - imageSize) / 2,
        imageSize,
        imageSize,
        IDB_AUDIO_INACTIVE,
        IDB_AUDIO,
        bind(&HeaderWindow::PlayText, this));

    audioButton->Initialize();
    this->childWindows.push_back(audioButton);

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

    PrintText(this->inMemoryHDC, headerText, fontHeader, colorBlack, paddingX, curY, &bottomRight);

    int originLineY = curY + AdjustToResolution(20, kY);
    POINT originLintBottomRight = PrintText(
        this->inMemoryHDC,
        subHeaderText,
        fontSmall,
        colorGray,
        paddingX + AdjustToResolution(16, kY) - int((kY-1)*10),
        originLineY,
        &bottomRight);

    if (translateResult.IsInputCorrected()) 
    {
        PrintText(
            this->inMemoryHDC, 
            L" (corrected from ",
            fontSmall,
            colorBlack,
            originLintBottomRight.x,
            originLineY,
            &originLintBottomRight);

        HoverTextButtonWindow* forceTranslationButton = new HoverTextButtonWindow(
            this->hWindow,
            this->hInstance,
            originLintBottomRight.x,
            originLineY,
            fontSmallUnderscored,
            colorGray,
            colorBlack,
            translateResult.Sentence.Input,
            bind(&HeaderWindow::PlayText, this));

        forceTranslationButton->Initialize();
        this->childWindows.push_back(forceTranslationButton);

        PrintText(
            this->inMemoryHDC,
            L")",
            fontSmall,
            colorGray,
            originLintBottomRight.x + forceTranslationButton->GetWidth(),
            originLineY,
            &bottomRight);
    }

    RECT rect;
    rect.left = 0;
    rect.right = 2000;
    rect.top = curY + 2 * lineHeight;
    rect.bottom = rect.top + 1;
    DrawRect(this->inMemoryHDC, rect, this->grayBrush, &POINT());

    bottomRight.x += paddingX * 3;
    bottomRight.y = rect.bottom;

    return bottomRight;
}

HeaderWindow::~HeaderWindow()
{
    DeleteObject(this->fontSmallUnderscored);
}