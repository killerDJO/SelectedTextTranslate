#include "Windows\Framework\RenderingContext.h"

RenderingContext::RenderingContext()
{
    ComputeScaleFactor();
}

void RenderingContext::ComputeScaleFactor()
{
    RECT workarea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);

    UINT screenResolutionX = workarea.right - workarea.left;
    UINT screenResolutionY = workarea.bottom - workarea.top;
    scaleFactor = screenResolutionY / 860.0;
    //scaleFactor = 1.5;
}

int RenderingContext::Scale(int x)
{
    return roundToInt(x * scaleFactor);
}

RECT RenderingContext::Scale(RECT rect)
{
    RECT scaledRect;
    scaledRect.left = Scale(rect.left);
    scaledRect.right = Scale(rect.right);
    scaledRect.top = Scale(rect.top);
    scaledRect.bottom = Scale(rect.bottom);
    return scaledRect;
}

WindowDescriptor RenderingContext::Scale(WindowDescriptor windowDescriptor)
{
    return WindowDescriptor::CreateWindowDescriptor(
        Scale(windowDescriptor.X),
        Scale(windowDescriptor.Y),
        Scale(windowDescriptor.Width),
        Scale(windowDescriptor.Height),
        windowDescriptor.OverflowX,
        windowDescriptor.OverflowY
    );
}

int RenderingContext::Downscale(int x)
{
    return roundToInt(x / scaleFactor);
}

SIZE RenderingContext::Downscale(SIZE size)
{
    SIZE downscaledSize;
    downscaledSize.cx = Downscale(size.cx);
    downscaledSize.cy = Downscale(size.cy);
    return downscaledSize;
}

SIZE RenderingContext::GetTextSize(HDC hdc, const wchar_t* text, HFONT font)
{
    SelectObject(hdc, font);

    SIZE textSize;
    GetTextExtentPoint32(hdc, text, wcslen(text), &textSize);

    return textSize;
}

HFONT RenderingContext::CreateCustomFont(HWND hWindow, FontSizes fontSize, bool isItalic, bool isUnderscored)
{
    HDC hdc = GetDC(hWindow);

    int fontSizeInPixels = 10;

    switch (fontSize)
    {
    case FontSizes::Normal:
        fontSizeInPixels = 10;
        break;
    case FontSizes::Large:
        fontSizeInPixels = 12;
        break;
    case FontSizes::Small:
        fontSizeInPixels = 8;
        break;
    }

    long logicalFontSize = -MulDiv(Scale(fontSizeInPixels), GetDeviceCaps(hdc, LOGPIXELSY), 72);

    int italicValue = isItalic ? 1 : 0;
    int underscoredValue = isUnderscored ? 1 : 0;

    HFONT font = CreateFont(logicalFontSize, 0, 0, 0, 0, isItalic, isUnderscored, 0, 0, 0, 0, 0, 0, TEXT("Arial"));

    DeleteDC(hdc);

    return font;
}

HBRUSH RenderingContext::CreateCustomBrush(Colors color)
{
    return CreateSolidBrush((COLORREF)color);
}

HDC RenderingContext::CreateInMemoryDC(DWORD width, DWORD height)
{
    HDC hdc = CreateCompatibleDC(NULL);

    BITMAPINFO i;
    ZeroMemory(&i.bmiHeader, sizeof(BITMAPINFOHEADER));
    i.bmiHeader.biWidth = width;
    i.bmiHeader.biHeight = height;
    i.bmiHeader.biPlanes = 1;
    i.bmiHeader.biBitCount = 24;
    i.bmiHeader.biSizeImage = 0;
    i.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    i.bmiHeader.biClrUsed = 0;
    i.bmiHeader.biClrImportant = 0;
    VOID *pvBits;
    HBITMAP bitmap = CreateDIBSection(hdc, &i, DIB_RGB_COLORS, &pvBits, NULL, 0);

    SelectObject(hdc, bitmap);

    return hdc;
}

void RenderingContext::ResizeDC(HDC &hdc, DWORD width, DWORD height)
{
    DeleteDC(hdc);
    hdc = CreateInMemoryDC(width, height);
}

void RenderingContext::ClearDC(HDC hdc, DWORD width, DWORD height)
{
    RECT rect;
    rect.top = 0;
    rect.left = 0;
    rect.bottom = height;
    rect.right = width;
    FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
}

DWORD RenderingContext::CopyDC(HDC source, HDC target, DWORD width, DWORD height)
{
    return BitBlt(target, 0, 0, width, height, source, 0, 0, SRCCOPY);
}

RenderingContext::~RenderingContext()
{
}