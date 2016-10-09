#include "Windows\Framework\RenderingContext.h"

RenderingContext::RenderingContext(ScaleProvider* scaleProvider, DeviceContextProvider* deviceContextProvider)
{
    this->scaleProvider = scaleProvider;
    this->deviceContextProvider = deviceContextProvider;
}

Size RenderingContext::GetTextSize(HDC deviceContext, const wchar_t* text, HFONT font) const
{
    SelectObject(deviceContext, font);

    SIZE textSize;
    GetTextExtentPoint32(deviceContext, text, wcslen(text), &textSize);

    return Size(textSize.cx, textSize.cy);
}

TEXTMETRIC RenderingContext::GetFontMetrics(HDC deviceContext, HFONT font) const
{
    SelectObject(deviceContext, font);

    TEXTMETRIC tm;
    GetTextMetrics(deviceContext, &tm);

    return tm;
}

Renderer* RenderingContext::GetRenderer()
{
    return new Renderer(this, deviceContextProvider, scaleProvider);
}

void RenderingContext::ReleaseRenderer(Renderer* renderer) const
{
    delete renderer;
}

HFONT RenderingContext::CreateCustomFont(HWND windowHandle, FontSizes fontSize, bool isItalic, bool isUnderscored) const
{
    HDC deviceContext = GetDC(windowHandle);

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

    long logicalFontSize = -MulDiv(scaleProvider->Scale(fontSizeInPixels), GetDeviceCaps(deviceContext, LOGPIXELSY), 72);

    int italicValue = isItalic ? 1 : 0;
    int underscoredValue = isUnderscored ? 1 : 0;

    HFONT font = CreateFont(logicalFontSize, 0, 0, 0, 0, italicValue, underscoredValue, 0, 0, 0, 0, PROOF_QUALITY, 0, TEXT("Arial"));

    DeleteDC(deviceContext);

    return font;
}

HBRUSH RenderingContext::CreateCustomBrush(Colors color) const
{
    return CreateSolidBrush((COLORREF)color);
}

RenderingContext::~RenderingContext()
{
}