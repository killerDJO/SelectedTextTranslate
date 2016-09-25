#pragma once
#include "Windows\Framework\Enums\FontSizes.h"
#include "Windows\Framework\Enums\Colors.h"
#include "Windows\Framework\Providers\ScaleProvider.h"
#include "Windows\Framework\Renderer.h"

class Renderer;

class RenderingContext
{
private:
    ScaleProvider* scaleProvider;

public:
    RenderingContext(ScaleProvider* scaleProvider);
    ~RenderingContext();

    HFONT CreateCustomFont(HWND hWindow, FontSizes fontSize, bool isItalic = false, bool isUnderscored = false) const;
    HBRUSH CreateCustomBrush(Colors color) const;
    Size GetTextSize(HDC hdc, const wchar_t* text, HFONT font) const;
    TEXTMETRIC GetFontMetrics(HDC hdc, HFONT font) const;

    Renderer* GetRenderer(HDC hdc);
    void ReleaseRenderer(Renderer* renderer) const;
};