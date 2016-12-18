#pragma once
#include "View\Framework\Rendering\DeviceContextBuffer.h"
#include "View\Framework\Rendering\RenderingContext.h"
#include "View\Framework\Rendering\Dto\TextRenderResult.h"
#include "View\Framework\Enums\Colors.h"
#include "View\Framework\Dto\Positioning\Size.h"
#include "View\Framework\Dto\Positioning\Rect.h"

class RenderingContext;
class Window;
class Brush;
class Font;

class Renderer
{
private:
    RenderingContext* renderingContext;

    Size originalSize;
    Brush* backgroundBrush;

    vector<function<void(HDC)>> renderActions;

    void ClearDeviceContext(HDC deviceContext, Size deviceContextSize) const;

public:
    Renderer(RenderingContext* renderingContext);
    ~Renderer();

    TextRenderResult PrintText(const wstring text, Font* font, Colors color, RenderPosition renderPosition, DWORD horizontalAlignment = TA_LEFT);
    void DrawRect(Rect rect, Brush* brush);
    void DrawBorderedRect(Rect rect, Brush* brush, int borderWidth, Colors borderColor);
    void SetBackground(Brush* backgroundBrush);

    Size GetSize() const;

    void IncreaseWidth(int widthToAdd);
    void IncreaseHeight(int heightToAdd);
    void UpdateRenderedContentSize(Window* window);

    void Render(HDC deviceContext, Size deviceContextSize);
    void Render(DeviceContextBuffer* deviceContextBuffer);
};