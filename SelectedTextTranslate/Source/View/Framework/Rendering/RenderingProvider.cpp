#include "View\Framework\Rendering\RenderingProvider.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

RenderingProvider::RenderingProvider(CompositionRoot* root)
{
    this->scaleProvider = root->GetService<ScaleProvider>();
    this->deviceContextProvider = root->GetService<DeviceContextProvider>();

    this->emptyDeviceContext = deviceContextProvider->CreateDeviceContext(Size(1, 1));
}

Size RenderingProvider::GetTextSize(wstring text, Font* font) const
{
    AssertCriticalWinApiResult(SelectObject(emptyDeviceContext, font->GetHandle()));

    SIZE textSize;
    AssertCriticalWinApiResult(GetTextExtentPoint32(emptyDeviceContext, text.c_str(), text.length(), &textSize));

    return Size(textSize.cx, textSize.cy);
}

TEXTMETRIC RenderingProvider::GetFontMetrics(HFONT font) const
{
    AssertCriticalWinApiResult(SelectObject(emptyDeviceContext, font));

    TEXTMETRIC tm;
    AssertCriticalWinApiResult(GetTextMetrics(emptyDeviceContext, &tm));

    return tm;
}

Font* RenderingProvider::CreateCustomFont(FontSizes fontSize, bool isItalic, bool isUnderscored, bool isBold) const
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

    TEXTMETRIC textmetric = GetFontMetrics(font);

    return new Font(font, textmetric.tmHeight, textmetric.tmAscent - textmetric.tmInternalLeading, textmetric.tmDescent, textmetric.tmAscent);
}

Brush* RenderingProvider::CreateCustomBrush(Colors color) const
{
    HBRUSH brush = CreateSolidBrush((COLORREF)color);
    AssertCriticalWinApiResult(brush);
    return new Brush(brush);
}

Pen* RenderingProvider::CreateCustomPen(Colors color, int strokeWidth) const
{
    HPEN pen = CreatePen(PS_SOLID, strokeWidth, (COLORREF)color);
    AssertCriticalWinApiResult(pen);
    return new Pen(pen);
}

RenderingProvider::~RenderingProvider()
{
    deviceContextProvider->DeleteDeviceContext(emptyDeviceContext);
}
