#pragma once
#include "View\Framework\Windows\Window.h"
#include "View\Framework\Enums\FontSizes.h"
#include "View\Framework\Enums\Colors.h"
#include "View\Framework\Providers\ScaleProvider.h"
#include "View\Framework\Renderer.h"

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

public:
    RenderingContext(ScaleProvider* scaleProvider, DeviceContextProvider* deviceContextProvider);
    ~RenderingContext();

    HFONT CreateCustomFont(FontSizes fontSize, bool isItalic = false, bool isUnderscored = false) const;
    HBRUSH CreateCustomBrush(Colors color) const;
    HPEN CreateCustomPen(Colors color, int strokeWidth) const;

    Size GetTextSize(wstring text, HFONT font) const;
    TEXTMETRIC GetFontMetrics(HFONT font) const;

    Renderer* GetRenderer();
    void ReleaseRenderer(Renderer* renderer) const;

    void BeginRender(Window* window);
    void EndRender(Window* window);
    bool IsRenderingRoot(Window* window) const;
};