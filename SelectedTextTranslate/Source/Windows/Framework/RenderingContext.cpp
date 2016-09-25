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
}

int RenderingContext::Scale(int value) const
{
    return roundToInt(value * scaleFactor);
}

Rect RenderingContext::Scale(Rect rect) const
{
    Rect scaledRect;
    scaledRect.X = Scale(rect.X);
    scaledRect.Y = Scale(rect.Y);
    scaledRect.Width = Scale(rect.Width);
    scaledRect.Height = Scale(rect.Height);
    return scaledRect;
}

Size RenderingContext::Scale(Size size) const
{
    return Size(Scale(size.Width), Scale(size.Height));
}

Point RenderingContext::Scale(Point point) const
{
    return Point(Scale(point.X), Scale(point.Y));
}

WindowDescriptor RenderingContext::Scale(WindowDescriptor windowDescriptor) const
{
    return WindowDescriptor::CreateWindowDescriptor(
        Scale(windowDescriptor.Position),
        Scale(windowDescriptor.WindowSize),
        windowDescriptor.OverflowX,
        windowDescriptor.OverflowY
    );
}

int RenderingContext::Downscale(int value) const
{
    return roundToInt(value / scaleFactor);
}

Size RenderingContext::Downscale(Size size) const
{
    Size downscaledSize;
    downscaledSize.Width = Downscale(size.Width);
    downscaledSize.Height = Downscale(size.Height);
    return downscaledSize;
}

int RenderingContext::Rescale(int value, double scaleFactorAjustment) const
{
    return roundToInt(Downscale(value) * (scaleFactor + scaleFactorAjustment));
}

void RenderingContext::AjustScaleFactor(double scaleFactorAjustment)
{
    scaleFactor += scaleFactorAjustment;
}

Size RenderingContext::GetTextSize(HDC hdc, const wchar_t* text, HFONT font) const
{
    SelectObject(hdc, font);

    SIZE textSize;
    GetTextExtentPoint32(hdc, text, wcslen(text), &textSize);

    return Size(textSize.cx, textSize.cy);
}

TEXTMETRIC RenderingContext::GetFontMetrics(HDC hdc, HFONT font) const
{
    SelectObject(hdc, font);

    TEXTMETRIC tm;
    GetTextMetrics(hdc, &tm);

    return tm;
}

HFONT RenderingContext::CreateCustomFont(HWND hWindow, FontSizes fontSize, bool isItalic, bool isUnderscored) const
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

    HFONT font = CreateFont(logicalFontSize, 0, 0, 0, 0, italicValue, underscoredValue, 0, 0, 0, 0, PROOF_QUALITY, 0, TEXT("Arial"));

    DeleteDC(hdc);

    return font;
}

HBRUSH RenderingContext::CreateCustomBrush(Colors color) const
{
    return CreateSolidBrush((COLORREF)color);
}

HDC RenderingContext::CreateInMemoryDC(Size dcSize) const
{
    HDC hdc = CreateCompatibleDC(nullptr);

    BITMAPINFO i;
    ZeroMemory(&i.bmiHeader, sizeof(BITMAPINFOHEADER));
    i.bmiHeader.biWidth = dcSize.Width;
    i.bmiHeader.biHeight = dcSize.Height;
    i.bmiHeader.biPlanes = 1;
    i.bmiHeader.biBitCount = 24;
    i.bmiHeader.biSizeImage = 0;
    i.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    i.bmiHeader.biClrUsed = 0;
    i.bmiHeader.biClrImportant = 0;
    VOID *pvBits;
    HBITMAP bitmap = CreateDIBSection(hdc, &i, DIB_RGB_COLORS, &pvBits, nullptr, 0);

    SelectObject(hdc, bitmap);

    return hdc;
}

void RenderingContext::ResizeDC(HDC &hdc, Size dcSize) const
{
    DeleteDC(hdc);
    hdc = CreateInMemoryDC(dcSize);
}

DWORD RenderingContext::CopyDC(HDC source, HDC target, Size dcSize) const
{
    return BitBlt(target, 0, 0, dcSize.Width, dcSize.Height, source, 0, 0, SRCCOPY);
}

RenderingContext::~RenderingContext()
{
}