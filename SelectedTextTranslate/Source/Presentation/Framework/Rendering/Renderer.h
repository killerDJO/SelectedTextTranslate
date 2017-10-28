#pragma once
#include "Presentation\Framework\Dto\Positioning\Size.h"
#include "Presentation\Framework\Dto\Positioning\Rect.h"
#include "Presentation\Framework\Enums\Colors.h"
#include "Presentation\Framework\Rendering\Dto\TextRenderResult.h"
#include "Presentation\Framework\Rendering\DeviceContextBuffer.h"
#include "Presentation\Framework\Rendering\RenderingProvider.h"
#include "Presentation\Framework\Views\View.h"
#include "Presentation\Framework\Components\IComponent.h"

class RenderingContext;
class View;

class Renderer
{
private:
    RenderingProvider* renderingProvider;
    DeviceContextProvider* deviceContextProvider;

    Brush* defaultBackgroundBrush;
    Brush* backgroundBrush;

    Size renderedSize;

    vector<function<void(HDC)>> renderActions;

    void ClearDeviceContext(HDC deviceContext, Size deviceContextSize) const;

public:
    Renderer(RenderingProvider* renderingProvider, DeviceContextProvider* deviceContextProvider);
    ~Renderer();

    TextRenderResult PrintText(const wstring text, Font* font, Colors color, RenderPosition renderPosition, DWORD horizontalAlignment = TA_LEFT);
    void DrawRect(Rect rect, Brush* brush);
    void DrawBorderedRect(Rect outerRect, Brush* brush, int borderWidth, Colors borderColor);
    void DrawDeviceContext(HDC deviceContext, Size contextSize);

    void SetBackground(Brush* backgroundBrush);
    const Brush* GetBackgroundBrush() const;

    Size GetSize() const;

    void IncreaseWidth(int widthToAdd);
    void IncreaseHeight(int heightToAdd);
    void UpdateRenderedContentSize(View* view);
    void UpdateRenderedContentSize(IComponent* component);
    void UpdateRenderedContentSize(Rect rect);
    void UpdateRenderedContentSize(Size size);

    void Render(HDC deviceContext, Size deviceContextSize);
    void Render(DeviceContextBuffer* deviceContextBuffer);
};