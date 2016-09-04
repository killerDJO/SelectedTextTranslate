#include "Windows\Content\Base\ContentWindow.h"

ContentWindow::ContentWindow(AppModel* appModel, HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height)
    : ChildWindow(parentWindow, hInstance, x, y, width, height)
{
    colorGray = RGB(119, 119, 119);
    colorBlack = RGB(0, 0, 0);

    this->appModel = appModel;
}

void ContentWindow::ComputeParameters()
{
    RECT size;
    GetWindowRect(this->parentWindow, &size);

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

    this->fontNormal = CreateFont(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
    this->fontHeader = CreateFont(lfHeightHeader, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
    this->fontItalic = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
    this->fontSmall = CreateFont(lfHeightSmall, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
}

void ContentWindow::InitializeBrushes()
{
    this->grayBrush = CreateSolidBrush(RGB(170, 170, 170));
}

POINT ContentWindow::RenderResult()
{
    POINT bottomRight = RenderDC();
    MoveWindow(this->hWindow, this->x, this->y, bottomRight.x, bottomRight.y, TRUE);
    //InvalidateRect(this->hWindow, NULL, FALSE);
    return bottomRight;
}

ContentWindow::~ContentWindow()
{
    DeleteObject(this->fontNormal);
    DeleteObject(this->fontHeader);
    DeleteObject(this->fontItalic);
    DeleteObject(this->fontSmall);
    DeleteObject(this->grayBrush);
}