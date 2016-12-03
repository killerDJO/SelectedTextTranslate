#pragma once
#include "View\Framework\Rendering\DeviceContextBuffer.h"
#include "View\Framework\Rendering\RenderingContext.h"
#include "View\Framework\Rendering\Dto\TextRenderResult.h"
#include "View\Framework\Providers\ScaleProvider.h"
#include "View\Framework\Enums\Colors.h"

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
    void DrawRect(Rect rect, HBRUSH brush);
    void DrawBorderedRect(Rect rect, HBRUSH brush, int borderWidth, Colors borderColor);
    void SetBackground(HBRUSH backgroundBrush);

    int GetFontAscent(HFONT font) const;
    int GetFontDescent(HFONT font) const;
    int GetFontStrokeHeight(HFONT font) const;
    int GetFontHeight(HFONT font) const;

    Size GetScaledSize() const;
    Size GetSize() const;

    void IncreaseWidth(int widthToAdd);
    void IncreaseHeight(int heightToAdd);
    void UpdateRenderedContentSize(Window* window);

    void Render(HDC deviceContext, Size deviceContextSize);
    void Render(DeviceContextBuffer* deviceContextBuffer);
};