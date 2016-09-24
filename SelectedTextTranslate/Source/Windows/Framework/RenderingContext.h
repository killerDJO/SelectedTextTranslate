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
    RECT Scale(RECT rect) const;

    int Downscale(int value) const;
    SIZE Downscale(SIZE size) const;

    int Rescale(int value, double scaleFactorAjustment) const;
    void AjustScaleFactor(double scaleFactorAjustment);

    HFONT CreateCustomFont(HWND hWindow, FontSizes fontSize, bool isItalic = false, bool isUnderscored = false) const;
    HBRUSH CreateCustomBrush(Colors color) const;
    SIZE GetTextSize(HDC hdc, const wchar_t* text, HFONT font) const;
    TEXTMETRIC GetFontMetrics(HDC hdc, HFONT font) const;

    HDC CreateInMemoryDC(DWORD width, DWORD height) const;
    void ResizeDC(HDC& hdc, DWORD width, DWORD height) const;
    DWORD CopyDC(HDC source, HDC target, DWORD width, DWORD height) const;
    void ClearDC(HDC hdc, DWORD width, DWORD height) const;
};