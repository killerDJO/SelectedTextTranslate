#pragma once
#include "View\Framework\Windows\Window.h"
#include "View\Framework\Enums\FontSizes.h"
#include "View\Framework\Enums\Colors.h"
#include "View\Framework\Providers\ScaleProvider.h"
#include "View\Framework\Rendering\Objects\Pen.h"
#include "View\Framework\Rendering\Objects\Font.h"
#include "View\Framework\Rendering\Objects\Brush.h"

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

    TEXTMETRIC GetFontMetrics(Font* font) const;

public:
    RenderingContext(ScaleProvider* scaleProvider, DeviceContextProvider* deviceContextProvider);

    Font* CreateCustomFont(FontSizes fontSize, bool isItalic = false, bool isUnderscored = false, bool isBold = false) const;
    Brush* CreateCustomBrush(Colors color) const;
    Pen* CreateCustomPen(Colors color, int strokeWidth) const;

    Size GetTextSize(wstring text, Font* font) const;

    int GetFontHeight(Font* font) const;
    int GetFontStrokeHeight(Font* font) const;
    int GetFontDescent(Font* font) const;
    int GetFontAscent(Font* font) const;

    Renderer* GetRenderer();
    void ReleaseRenderer(Renderer* renderer) const;

    void BeginRender(Window* window);
    void EndRender(Window* window);
    bool IsRenderingRoot(Window* window) const;
};