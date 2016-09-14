#include "Windows\Content\Base\ContentWindow.h"

ContentWindow::ContentWindow(Renderer* renderer, AppModel* appModel, HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height)
    : ChildWindow(renderer, parentWindow, hInstance, x, y, width, height)
{
    this->appModel = appModel;

    this->dcHeight = 0;
    this->dcWidth = 0;

    colorGray = RGB(119, 119, 119);
    colorBlack = RGB(0, 0, 0);
    colorLightGray = RGB(170, 170, 170);

    lineHeight = renderer->AdjustToYResolution(20);
    paddingX = renderer->AdjustToXResolution(15);
    paddingY = renderer->AdjustToYResolution(15);
}

void ContentWindow::InitializeInMemoryDC()
{
    inMemoryDC = renderer->CreateInMemoryDC(dcWidth, dcHeight);
}

void ContentWindow::InitializeFonts()
{
    HDC hdc = GetDC(hWindow);

    long lfHeight = -MulDiv(fontHeight / 2, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    long lfHeightHeader = -MulDiv(roundToInt(fontHeight * 3 / 5.0), GetDeviceCaps(hdc, LOGPIXELSY), 72);
    long lfHeightSmall = -MulDiv(roundToInt(fontHeight * 3 / 7.0), GetDeviceCaps(hdc, LOGPIXELSY), 72);

    fontNormal = CreateFont(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
    fontHeader = CreateFont(lfHeightHeader, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
    fontItalic = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
    fontSmall = CreateFont(lfHeightSmall, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
}

void ContentWindow::InitializeBrushes()
{
    grayBrush = CreateSolidBrush(colorLightGray);
}

POINT ContentWindow::RenderResult()
{
    POINT bottomRight = RenderDC();

    // Check if current buffer DC has enough size
    if (bottomRight.x > (int)dcWidth || bottomRight.y > (int)dcHeight)
    {
        dcWidth = bottomRight.x;
        dcHeight = bottomRight.y;
        renderer->ResizeDC(inMemoryDC, dcWidth, dcHeight);
        RenderDC();
    }

    width = bottomRight.x;
    height = bottomRight.y;

    InvalidateRect(hWindow, NULL, FALSE);
    MoveWindow(hWindow, x, y, width, height, FALSE);
    return bottomRight;
}

POINT ContentWindow::RenderDC()
{
    POINT bottomRight = ChildWindow::RenderDC();
    renderer->ClearDC(inMemoryDC, dcWidth, dcHeight);
    return bottomRight;
}

ContentWindow::~ContentWindow()
{
    DeleteObject(fontNormal);
    DeleteObject(fontHeader);
    DeleteObject(fontItalic);
    DeleteObject(fontSmall);
    DeleteObject(grayBrush);
}