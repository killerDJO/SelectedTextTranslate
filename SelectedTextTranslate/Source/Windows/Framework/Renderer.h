#pragma once
#include "Windows\Framework\RenderingContext.h"
#include "Windows\Framework\Providers\DeviceContextProvider.h"
#include "Windows\Framework\DeviceContextBuffer.h"

class RenderingContext;
class ScrollProvider;

class Renderer
{
private:
    RenderingContext* renderingContext;
    ScaleProvider* scaleProvider;
    ScrollProvider* scrollProvider;

    HDC emptyDeviceContext;
    Size originalSize;

    vector<function<void(HDC)>> renderActions;

    void ClearDeviceContext(HDC deviceContext, Size deviceContextSize) const;

public:
    Renderer(RenderingContext* renderingContext, DeviceContextProvider* deviceContextProvider, ScaleProvider* scaleProvider, ScrollProvider* scrollProvider);
    ~Renderer();

    Point PrintText(const wchar_t* text, HFONT font, Colors color, Point position);
    void DrawRect(Rect rect, HBRUSH brush);

    int GetFontAscent(HFONT font) const;
    int GetFontStrokeHeight(HFONT font) const;

    Size GetScaledSize() const;
    Size GetSize() const;

    void IncreaseWidth(int widthToAdd);
    void IncreaseHeight(int heightToAdd);

    void Render(HDC deviceContext, Size deviceContextSize);
    void Render(DeviceContextBuffer* deviceContextBuffer);
};