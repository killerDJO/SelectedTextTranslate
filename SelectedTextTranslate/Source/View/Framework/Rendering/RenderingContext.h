#pragma once
#include "View\Framework\Windows\Window.h"
#include "View\Framework\Enums\FontSizes.h"
#include "View\Framework\Enums\Colors.h"
#include "View\Framework\Providers\ScaleProvider.h"

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

    void DeleteCustomObject(HGDIOBJ gdiObject) const;
    TEXTMETRIC GetFontMetrics(HFONT font) const;

public:
    RenderingContext(ScaleProvider* scaleProvider, DeviceContextProvider* deviceContextProvider);
    ~RenderingContext();

    HFONT CreateCustomFont(FontSizes fontSize, bool isItalic = false, bool isUnderscored = false, bool isBold = false) const;
    void DeleteCustomFont(HFONT font) const;

    HBRUSH CreateCustomBrush(Colors color) const;
    void DeleteCustomBrush(HBRUSH brush) const;

    HPEN CreateCustomPen(Colors color, int strokeWidth) const;
    void DeleteCustomPen(HPEN pen) const;

    Size GetTextSize(wstring text, HFONT font) const;

    int GetFontHeight(HFONT font) const;
    int GetFontStrokeHeight(HFONT font) const;
    int GetFontDescent(HFONT font) const;
    int GetFontAscent(HFONT font) const;

    Renderer* GetRenderer();
    void ReleaseRenderer(Renderer* renderer) const;

    void BeginRender(Window* window);
    void EndRender(Window* window);
    bool IsRenderingRoot(Window* window) const;
};