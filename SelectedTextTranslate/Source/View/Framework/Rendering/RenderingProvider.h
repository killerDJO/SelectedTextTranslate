#pragma once
#include "View\Framework\Enums\FontSizes.h"
#include "View\Framework\Enums\Colors.h"
#include "View\Framework\Providers\ScaleProvider.h"
#include "View\Framework\Providers\DeviceContextProvider.h"
#include "View\Framework\Rendering\Objects\Pen.h"
#include "View\Framework\Rendering\Objects\Font.h"
#include "View\Framework\Rendering\Objects\Brush.h"
#include "CompositionRoot.h"

class RenderingProvider
{
private:
    ScaleProvider* scaleProvider;
    DeviceContextProvider* deviceContextProvider;

    HDC emptyDeviceContext;

    TEXTMETRIC GetFontMetrics(HFONT font) const;

public:
    RenderingProvider(CompositionRoot* root);
    ~RenderingProvider();

    Font* CreateCustomFont(FontSizes fontSize, bool isItalic = false, bool isUnderscored = false, bool isBold = false) const;
    Brush* CreateCustomBrush(Colors color) const;
    Pen* CreateCustomPen(Colors color, int strokeWidth) const;

    Size GetTextSize(wstring text, Font* font) const;
};
