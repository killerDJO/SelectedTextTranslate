#include "View\Framework\RenderingContext.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

RenderingContext::RenderingContext(ScaleProvider* scaleProvider, DeviceContextProvider* deviceContextProvider, ScrollProvider* scrollProvider)
{
    this->scaleProvider = scaleProvider;
    this->deviceContextProvider = deviceContextProvider;
    this->scrollProvider = scrollProvider;

    this->renderingRoot = nullptr;
    this->emptyDeviceContext = deviceContextProvider->CreateDeviceContext(Size(1, 1));
}

Size RenderingContext::GetTextSize(wstring text, HFONT font) const
{
    AssertCriticalWinApiResult(SelectObject(emptyDeviceContext, font));

    SIZE textSize;
    AssertCriticalWinApiResult(GetTextExtentPoint32(emptyDeviceContext, text.c_str(), text.length(), &textSize));

    return Size(textSize.cx, textSize.cy);
}

TEXTMETRIC RenderingContext::GetFontMetrics(HFONT font) const
{
    AssertCriticalWinApiResult(SelectObject(emptyDeviceContext, font));

    TEXTMETRIC tm;
    AssertCriticalWinApiResult(GetTextMetrics(emptyDeviceContext, &tm));

    return tm;
}

Renderer* RenderingContext::GetRenderer()
{
    return new Renderer(this, deviceContextProvider, scaleProvider, scrollProvider);
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

HFONT RenderingContext::CreateCustomFont(FontSizes fontSize, bool isItalic, bool isUnderscored) const
{
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

    long logicalFontSize = -MulDiv(scaleProvider->Scale(fontSizeInPixels), GetDeviceCaps(emptyDeviceContext, LOGPIXELSY), 72);
    ExceptionHelper::ThrowOnWinapiError(logicalFontSize, true, -1);

    int italicValue = isItalic ? 1 : 0;
    int underscoredValue = isUnderscored ? 1 : 0;

    HFONT font = CreateFont(logicalFontSize, 0, 0, 0, 0, italicValue, underscoredValue, 0, 0, 0, 0, PROOF_QUALITY, 0, TEXT("Arial"));
    AssertCriticalWinApiResult(font);

    return font;
}

HBRUSH RenderingContext::CreateCustomBrush(Colors color) const
{
    HBRUSH brush = CreateSolidBrush((COLORREF)color);
    AssertCriticalWinApiResult(brush);
    return brush;
}

HPEN RenderingContext::CreateCustomPen(Colors color, int strokeWidth) const
{
    HPEN pen = CreatePen(PS_SOLID, scaleProvider->Scale(strokeWidth), (COLORREF)color);
    AssertCriticalWinApiResult(pen);
    return pen;
}

RenderingContext::~RenderingContext()
{
}
