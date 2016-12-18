#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "View\Framework\Rendering\RenderingContext.h"

RenderingContext::RenderingContext(ScaleProvider* scaleProvider, DeviceContextProvider* deviceContextProvider)
{
    this->scaleProvider = scaleProvider;
    this->deviceContextProvider = deviceContextProvider;

    this->renderingRoot = nullptr;
    this->emptyDeviceContext = deviceContextProvider->CreateDeviceContext(Size(1, 1));
}

Size RenderingContext::GetTextSize(wstring text, Font* font) const
{
    AssertCriticalWinApiResult(SelectObject(emptyDeviceContext, font->GetHandle()));

    SIZE textSize;
    AssertCriticalWinApiResult(GetTextExtentPoint32(emptyDeviceContext, text.c_str(), text.length(), &textSize));

    return Size(textSize.cx, textSize.cy);
}

TEXTMETRIC RenderingContext::GetFontMetrics(Font* font) const
{
    AssertCriticalWinApiResult(SelectObject(emptyDeviceContext, font->GetHandle()));

    TEXTMETRIC tm;
    AssertCriticalWinApiResult(GetTextMetrics(emptyDeviceContext, &tm));

    return tm;
}

int RenderingContext::GetFontHeight(Font* font) const
{
    return GetFontMetrics(font).tmHeight;
}

int RenderingContext::GetFontStrokeHeight(Font* font) const
{
    TEXTMETRIC textmetric = GetFontMetrics(font);
    return textmetric.tmAscent - textmetric.tmInternalLeading;
}

int RenderingContext::GetFontDescent(Font* font) const
{
    return GetFontMetrics(font).tmDescent;
}

int RenderingContext::GetFontAscent(Font* font) const
{
    return GetFontMetrics(font).tmAscent;
}

Renderer* RenderingContext::GetRenderer()
{
    return new Renderer(this, scaleProvider);
}

void RenderingContext::ReleaseRenderer(Renderer* renderer) const
{
    delete renderer;
}

void RenderingContext::BeginRender(Window* window)
{
    if(renderingRoot == nullptr)
    {
        renderingRoot = window;
    }
}

void RenderingContext::EndRender(Window* window)
{
    if(window == renderingRoot)
    {
        renderingRoot = nullptr;
    }
}

bool RenderingContext::IsRenderingRoot(Window* window) const
{
    return window == renderingRoot;
}

Font* RenderingContext::CreateCustomFont(FontSizes fontSize, bool isItalic, bool isUnderscored, bool isBold) const
{
    int fontSizeInPixels = 10;

    switch (fontSize)
    {
    case FontSizes::Medium:
        fontSizeInPixels = 9;
        break;
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

    long logicalFontSize = -MulDiv(scaleProvider->Scale(fontSizeInPixels), GetDeviceCaps(emptyDeviceContext, LOGPIXELSY), 72);
    ExceptionHelper::ThrowOnWinapiError(logicalFontSize, true, -1);

    int italicValue = isItalic ? 1 : 0;
    int underscoredValue = isUnderscored ? 1 : 0;
    int boldValue = isBold ? FW_BOLD : 0;

    HFONT font = CreateFont(logicalFontSize, 0, 0, 0, boldValue, italicValue, underscoredValue, 0, 0, 0, 0, PROOF_QUALITY, 0, TEXT("Arial"));
    AssertCriticalWinApiResult(font);

    return new Font(font);
}

Brush* RenderingContext::CreateCustomBrush(Colors color) const
{
    HBRUSH brush = CreateSolidBrush((COLORREF)color);
    AssertCriticalWinApiResult(brush);
    return new Brush(brush);
}

Pen* RenderingContext::CreateCustomPen(Colors color, int strokeWidth) const
{
    HPEN pen = CreatePen(PS_SOLID, strokeWidth, (COLORREF)color);
    AssertCriticalWinApiResult(pen);
    return new Pen(pen);
}