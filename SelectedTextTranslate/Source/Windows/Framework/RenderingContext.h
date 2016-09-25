#pragma once
#include "Windows\Framework\Enums\FontSizes.h"
#include "Windows\Framework\Enums\Colors.h"
#include "Windows\Framework\Dto\WindowDescriptor.h"

class RenderingContext
{
private:
    double scaleFactor;

    void ComputeScaleFactor();

public:

    RenderingContext();
    ~RenderingContext();

    int Scale(int value) const;
    WindowDescriptor Scale(WindowDescriptor windowDescriptor) const;
    Rect Scale(Rect rect) const;
    Size Scale(Size size) const;
    Point Scale(Point point) const;

    int Downscale(int value) const;
    Size Downscale(Size dcSize) const;

    int Rescale(int value, double scaleFactorAjustment) const;
    void AjustScaleFactor(double scaleFactorAjustment);

    HFONT CreateCustomFont(HWND hWindow, FontSizes fontSize, bool isItalic = false, bool isUnderscored = false) const;
    HBRUSH CreateCustomBrush(Colors color) const;
    Size GetTextSize(HDC hdc, const wchar_t* text, HFONT font) const;
    TEXTMETRIC GetFontMetrics(HDC hdc, HFONT font) const;

    HDC CreateInMemoryDC(Size dcSize) const;
    void ResizeDC(HDC& hdc, Size newDcSize) const;
    DWORD CopyDC(HDC source, HDC target, Size dcSize) const;
};