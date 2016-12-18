#pragma once
#include "View\Framework\Rendering\DeviceContextBuffer.h"
#include "View\Framework\Rendering\RenderingContext.h"
#include "View\Framework\Rendering\Dto\TextRenderResult.h"
#include "View\Framework\Providers\ScaleProvider.h"
#include "View\Framework\Enums\Colors.h"
#include "View\Framework\Dto\Size\SizeReal.h"
#include "View\Framework\Dto\Rect\RectReal.h"

class RenderingContext;
class ScrollProvider;
class Window;

class Renderer
{
private:
    RenderingContext* renderingContext;
    ScaleProvider* scaleProvider;

    Size originalSize;

    HBRUSH backgroundBrush;

    vector<function<void(HDC)>> renderActions;

    void ClearDeviceContext(HDC deviceContext, Size deviceContextSize) const;

public:
    Renderer(RenderingContext* renderingContext, ScaleProvider* scaleProvider);
    ~Renderer();

    TextRenderResult PrintText(const wstring text, HFONT font, Colors color, RenderPosition renderPosition, DWORD horizontalAlignment = TA_LEFT);
    void DrawRect(RectReal rect, HBRUSH brush);
    void DrawBorderedRect(RectReal rect, HBRUSH brush, double borderWidth, Colors borderColor);
    void SetBackground(HBRUSH backgroundBrush);

    double GetFontAscent(HFONT font) const;
    double GetFontDescent(HFONT font) const;
    double GetFontStrokeHeight(HFONT font) const;
    double GetFontHeight(HFONT font) const;

    Size GetScaledSize() const;
    SizeReal GetSize() const;

    void IncreaseWidth(double widthToAdd);
    void IncreaseHeight(double heightToAdd);
    void UpdateRenderedContentSize(Window* window);

    void Render(HDC deviceContext, Size deviceContextSize);
    void Render(DeviceContextBuffer* deviceContextBuffer);
};