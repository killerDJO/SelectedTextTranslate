#pragma once
#include "View\Framework\Enums\FontSizes.h"
#include "View\Framework\Enums\Colors.h"
#include "View\Framework\Providers\ScaleProvider.h"
#include "View\Framework\Rendering\Objects\Pen.h"
#include "View\Framework\Rendering\Objects\Font.h"
#include "View\Framework\Rendering\Objects\Brush.h"
#include "View\Framework\Windows\Window.h"

class Renderer;
class Window;
class DeviceContextProvider;

class RenderingContext
{
private:
    ScaleProvider* scaleProvider;
    DeviceContextProvider* deviceContextProvider;

    Window* renderingRoot;
    HDC emptyDeviceContext;

    TEXTMETRIC GetFontMetrics(HFONT font) const;

public:
    RenderingContext(ScaleProvider* scaleProvider, DeviceContextProvider* deviceContextProvider);
    ~RenderingContext();

    Font* CreateCustomFont(FontSizes fontSize, bool isItalic = false, bool isUnderscored = false, bool isBold = false) const;
    Brush* CreateCustomBrush(Colors color) const;
    Pen* CreateCustomPen(Colors color, int strokeWidth) const;

    Size GetTextSize(wstring text, Font* font) const;

    Renderer* GetRenderer();
    void ReleaseRenderer(Renderer* renderer) const;

    void BeginRender(Window* window);
    void EndRender(Window* window);
    bool IsRenderingRoot(Window* window) const;
};