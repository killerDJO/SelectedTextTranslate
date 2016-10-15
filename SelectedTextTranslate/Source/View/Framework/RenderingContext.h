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
    ScrollProvider* scrollProvider;

    Window* renderingRoot;

public:
    RenderingContext(ScaleProvider* scaleProvider, DeviceContextProvider* deviceContextProvider, ScrollProvider* scrollProvider);
    ~RenderingContext();

    HFONT CreateCustomFont(HWND windowHandle, FontSizes fontSize, bool isItalic = false, bool isUnderscored = false) const;
    HBRUSH CreateCustomBrush(Colors color) const;

    Size GetTextSize(HDC deviceContext, const wchar_t* text, HFONT font) const;
    TEXTMETRIC GetFontMetrics(HDC deviceContext, HFONT font) const;

    Renderer* GetRenderer();
    void ReleaseRenderer(Renderer* renderer) const;

    void BeginRender(Window* window);
    void EndRender(Window* window);
    bool IsRenderingRoot(Window* window) const;
};