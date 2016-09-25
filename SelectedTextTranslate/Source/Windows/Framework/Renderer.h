#pragma once
#include "Windows\Framework\RenderingContext.h"

class Renderer
{
private:
    RenderingContext* renderingContext;
    HDC hdc;

    Size scaledSize;

public:
    Renderer(HDC hdc, RenderingContext* renderingContext);
    ~Renderer();

    Point PrintText(const wchar_t* text, HFONT font, Colors color, Point position);
    void DrawRect(Rect rect, HBRUSH brush);
    void ClearDC(HDC hdc, Size size) const;
    int GetFontAscent(HFONT font) const;
    int GetFontStrokeHeight(HFONT font) const;

    Size GetScaledSize() const;
    Size GetSize() const;

    void IncreaseWidth(int widthToAdd);
    void IncreaseHeight(int heightToAdd);
};