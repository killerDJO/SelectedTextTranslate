#include "Windows\Framework\RenderingContext.h"

RenderingContext::RenderingContext(ScaleProvider* scaleProvider)
{
    this->scaleProvider = scaleProvider;
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

Renderer* RenderingContext::GetRenderer(HDC hdc)
{
    return new Renderer(hdc, this, scaleProvider);
}

void RenderingContext::ReleaseRenderer(Renderer* renderer) const
{
    delete renderer;
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

    long logicalFontSize = -MulDiv(scaleProvider->Scale(fontSizeInPixels), GetDeviceCaps(hdc, LOGPIXELSY), 72);

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

RenderingContext::~RenderingContext()
{
}