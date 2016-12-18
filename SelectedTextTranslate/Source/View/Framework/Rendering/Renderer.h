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
class Brush;
class Font;

class Renderer
{
private:
    RenderingContext* renderingContext;
    ScaleProvider* scaleProvider;

    Size originalSize;

    Brush* backgroundBrush;

    vector<function<void(HDC)>> renderActions;

    void ClearDeviceContext(HDC deviceContext, Size deviceContextSize) const;

public:
    Renderer(RenderingContext* renderingContext, ScaleProvider* scaleProvider);
    ~Renderer();

    TextRenderResult PrintText(const wstring text, Font* font, Colors color, RenderPosition renderPosition, DWORD horizontalAlignment = TA_LEFT);
    void DrawRect(RectReal rect, Brush* brush);
    void DrawBorderedRect(RectReal rect, Brush* brush, double borderWidth, Colors borderColor);
    void SetBackground(Brush* backgroundBrush);

    double GetFontAscent(Font* font) const;
    double GetFontDescent(Font* font) const;
    double GetFontStrokeHeight(Font* font) const;
    double GetFontHeight(Font* font) const;

    Size GetScaledSize() const;
    SizeReal GetSize() const;

    void IncreaseWidth(double widthToAdd);
    void IncreaseHeight(double heightToAdd);
    void UpdateRenderedContentSize(Window* window);

    void Render(HDC deviceContext, Size deviceContextSize);
    void Render(DeviceContextBuffer* deviceContextBuffer);
};