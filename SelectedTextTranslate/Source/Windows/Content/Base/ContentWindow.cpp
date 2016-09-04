#include "Windows\Content\Base\ContentWindow.h"

ContentWindow::ContentWindow(AppModel* appModel, HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height)
    : ChildWindow(parentWindow, hInstance, x, y, width, height)
{
    colorGray = RGB(119, 119, 119);
    colorBlack = RGB(0, 0, 0);
    colorLightGray = RGB(170, 170, 170);

    this->appModel = appModel;
}

void ContentWindow::ComputeParameters()
{
    RECT size;
    GetWindowRect(parentWindow, &size);

    UINT width = size.right - size.left;
    UINT height = size.bottom - size.top;

    kX = width / 300.0;
    kY = height / 400.0;

    lineHeight = AdjustToResolution(20, kY);
    paddingX = AdjustToResolution(15, kX);
    paddingY = AdjustToResolution(15, kY);
}

void ContentWindow::InitializeFonts()
{
    HDC hdc = GetDC(hWindow);

    long lfHeight = -MulDiv(fontHeight / 2, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    long lfHeightHeader = -MulDiv(int(fontHeight * 3 / 5.0), GetDeviceCaps(hdc, LOGPIXELSY), 72);
    long lfHeightSmall = -MulDiv(int(fontHeight * 3 / 7.0), GetDeviceCaps(hdc, LOGPIXELSY), 72);

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
    InvalidateRect(hWindow, NULL, FALSE);
    MoveWindow(hWindow, x, y, width, height, FALSE);
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