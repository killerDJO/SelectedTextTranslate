#pragma once
#include "View\Framework\RenderingContext.h"
#include "View\Framework\DeviceContextBuffer.h"
#include "View\Framework\Providers\DeviceContextProvider.h"

class RenderingContext;
class ScrollProvider;

class Renderer
{
private:
    RenderingContext* renderingContext;
    ScaleProvider* scaleProvider;
    ScrollProvider* scrollProvider;

    HDC emptyDeviceContext;
    HBRUSH backgroundBrush;
    Size originalSize;

    vector<function<void(HDC)>> renderActions;

    void ClearDeviceContext(HDC deviceContext, Size deviceContextSize) const;

public:
    Renderer(RenderingContext* renderingContext, DeviceContextProvider* deviceContextProvider, ScaleProvider* scaleProvider, ScrollProvider* scrollProvider);
    ~Renderer();

    Point PrintText(const wstring text, HFONT font, Colors color, Point position);
    void DrawFilledRect(Rect rect, HBRUSH brush);
    void DrawRectUnscaled(Rect rect, HPEN pen, HBRUSH brush = nullptr);
    void SetBackground(HBRUSH backgroundBrush);

    int GetFontAscent(HFONT font) const;
    int GetFontStrokeHeight(HFONT font) const;
    int GetFontHeight(HFONT font) const;

    Size GetScaledSize() const;
    Size GetSize() const;

    void IncreaseWidth(int widthToAdd);
    void IncreaseHeight(int heightToAdd);
    void UpdateSize(Size size);

    void Render(HDC deviceContext, Size deviceContextSize);
    void Render(DeviceContextBuffer* deviceContextBuffer);
};