#include "View\Framework\RenderingContext.h"
#include "Helpers\ExceptionHelper.h"

RenderingContext::RenderingContext(ScaleProvider* scaleProvider, DeviceContextProvider* deviceContextProvider, ScrollProvider* scrollProvider)
{
    this->scaleProvider = scaleProvider;
    this->deviceContextProvider = deviceContextProvider;
    this->scrollProvider = scrollProvider;

    this->renderingRoot = nullptr;
}

Size RenderingContext::GetTextSize(HDC deviceContext, const wchar_t* text, HFONT font) const
{
    AssertCriticalWinApiResult(SelectObject(deviceContext, font));

    SIZE textSize;
    AssertCriticalWinApiResult(GetTextExtentPoint32(deviceContext, text, wcslen(text), &textSize));

    return Size(textSize.cx, textSize.cy);
}

TEXTMETRIC RenderingContext::GetFontMetrics(HDC deviceContext, HFONT font) const
{
    AssertCriticalWinApiResult(SelectObject(deviceContext, font));

    TEXTMETRIC tm;
    AssertCriticalWinApiResult(GetTextMetrics(deviceContext, &tm));

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

HFONT RenderingContext::CreateCustomFont(HWND windowHandle, FontSizes fontSize, bool isItalic, bool isUnderscored) const
{
    HDC deviceContext = GetDC(windowHandle);
    AssertCriticalWinApiResult(deviceContext);

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
    ExceptionHelper::ThrowOnWinapiError(logicalFontSize, __WFILE__, __LINE__, true, -1);

    int italicValue = isItalic ? 1 : 0;
    int underscoredValue = isUnderscored ? 1 : 0;

    HFONT font = CreateFont(logicalFontSize, 0, 0, 0, 0, italicValue, underscoredValue, 0, 0, 0, 0, PROOF_QUALITY, 0, TEXT("Arial"));
    AssertCriticalWinApiResult(font);

    AssertCriticalWinApiResult(DeleteDC(deviceContext));

    return font;
}

HBRUSH RenderingContext::CreateCustomBrush(Colors color) const
{
    HBRUSH brush = CreateSolidBrush((COLORREF)color);
    AssertCriticalWinApiResult(brush);
    return brush;
}

RenderingContext::~RenderingContext()
{
}