#pragma once
#include "Windows\Framework\RenderingContext.h"

class Renderer
{
private:
    RenderingContext* renderingContext;
    HDC hdc;

    SIZE scaledSize;

public:
    Renderer(HDC hdc, RenderingContext* renderingContext);
    ~Renderer();

    POINT PrintText(const wchar_t* text, HFONT font, Colors color, int x, int y);
    void DrawRect(RECT rect, HBRUSH brush);
    int GetFontAscent(HFONT font);
    int GetFontStrokeHeight(HFONT font);

    SIZE GetScaledSize();
    SIZE GetSize();

    void IncreaseWidth(int widthToAdd);
    void IncreaseHeight(int heightToAdd);
};