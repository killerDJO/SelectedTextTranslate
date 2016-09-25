#include "Windows\Framework\Renderer.h"

Renderer::Renderer(HDC hdc, RenderingContext* renderingContext)
{
    this->hdc = hdc;
    this->renderingContext = renderingContext;
    this->scaledSize = Size(0, 0);
}

Point Renderer::PrintText(const wchar_t* text, HFONT font, Colors color, Point position)
{
    Point scaledPosition = renderingContext->Scale(position);

    SelectObject(hdc, font);
    SetTextColor(hdc, (COLORREF)color);

    SetTextAlign(hdc, TA_BASELINE | TA_LEFT);
    TextOut(hdc, scaledPosition.X, scaledPosition.Y, text, wcslen(text));

    Size textSize = renderingContext->GetTextSize(hdc, text, font);

    scaledSize.Width = max(scaledSize.Width, scaledPosition.X + textSize.Width);
    scaledSize.Height = max(scaledSize.Height, scaledPosition.Y + textSize.Height);

    Size downscaledSize = renderingContext->Downscale(textSize);

    Point result;
    result.X = position.X + downscaledSize.Width;
    result.Y = position.Y - GetFontAscent(font) + downscaledSize.Height;

    return result;
}

void Renderer::DrawRect(Rect rect, HBRUSH brush)
{
    Rect scaledRect = renderingContext->Scale(rect);

    RECT gdiRect;
    gdiRect.left = scaledRect.X;
    gdiRect.right = scaledRect.GetRight();
    gdiRect.top = scaledRect.Y;
    gdiRect.bottom = scaledRect.GetBottom();
    FillRect(hdc, &gdiRect, brush);

    scaledSize.Width = max(scaledSize.Width, scaledRect.Width);
    scaledSize.Height = max(scaledSize.Height, scaledRect.Height);
}

void Renderer::ClearDC(HDC hdc, Size dcSize) const
{
    RECT rect;
    rect.top = 0;
    rect.left = 0;
    rect.bottom = dcSize.Height;
    rect.right = dcSize.Width;
    FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
}

int Renderer::GetFontAscent(HFONT font) const
{
    return renderingContext->Downscale(renderingContext->GetFontMetrics(hdc, font).tmAscent);
}

int Renderer::GetFontStrokeHeight(HFONT font) const
{
    TEXTMETRIC textMetrics = renderingContext->GetFontMetrics(hdc, font);
    return renderingContext->Downscale(textMetrics.tmAscent - textMetrics.tmInternalLeading);
}

Size Renderer::GetScaledSize() const
{
    return scaledSize;
}

Size Renderer::GetSize() const
{
    return renderingContext->Downscale(scaledSize);
}

void Renderer::IncreaseWidth(int widthToAdd)
{
    scaledSize.Width += renderingContext->Scale(widthToAdd);
}

void Renderer::IncreaseHeight(int heightToAdd)
{
    scaledSize.Height += renderingContext->Scale(heightToAdd);
}

Renderer::~Renderer()
{
}
