#pragma once
#include "Presentation\Framework\Enums\FontSizes.h"
#include "Presentation\Framework\Enums\Colors.h"
#include "Presentation\Framework\Providers\ScaleProvider.h"
#include "Presentation\Framework\Providers\DeviceContextProvider.h"
#include "Presentation\Framework\Rendering\Objects\Pen.h"
#include "Presentation\Framework\Rendering\Objects\Font.h"
#include "Presentation\Framework\Rendering\Objects\Brush.h"
#include "Infrastructure\CompositionRoot.h"

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
