#include "Windows\Framework\Renderer.h"

Renderer::Renderer(HDC hdc, RenderingContext* renderingContext)
{
    this->hdc = hdc;
    this->renderingContext = renderingContext;
}

POINT Renderer::PrintText(const wchar_t* text, HFONT font, Colors color, int x, int y)
{
    int scaledX = renderingContext->Scale(x);
    int scaledY = renderingContext->Scale(y);

    SelectObject(hdc, font);
    SetTextColor(hdc, (COLORREF)color);

    TextOut(hdc, scaledX, scaledY, text, _tcslen(text));

    SIZE textSize;
    GetTextExtentPoint32(hdc, text, wcslen(text), &textSize);

    scaledSize.cx = max(scaledSize.cx, scaledX + textSize.cx);
    scaledSize.cy = max(scaledSize.cy, scaledY + textSize.cy);

    SIZE downscaledSize = renderingContext->Downscale(textSize);

    POINT result;
    result.x = x + downscaledSize.cx;
    result.y = y + downscaledSize.cy;

    return result;
}

void Renderer::DrawRect(RECT rect, HBRUSH brush)
{
    RECT scaledRect = renderingContext->Scale(rect);
    FillRect(hdc, &scaledRect, brush);

    scaledSize.cx = max(scaledSize.cx, scaledRect.right);
    scaledSize.cy = max(scaledSize.cy, scaledRect.bottom);
}

SIZE Renderer::GetScaledSize()
{
    return scaledSize;
}

SIZE Renderer::GetSize()
{
    return renderingContext->Downscale(scaledSize);
}

void Renderer::IncreaseWidth(int widthToAdd)
{
    scaledSize.cx += renderingContext->Scale(widthToAdd);
}

void Renderer::IncreaseHeight(int heightToAdd)
{
    scaledSize.cy += renderingContext->Scale(heightToAdd);
}

Renderer::~Renderer()
{
}
