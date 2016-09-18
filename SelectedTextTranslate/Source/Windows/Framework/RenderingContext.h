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

    int Scale(int value);
    WindowDescriptor Scale(WindowDescriptor windowDescriptor);
    RECT Scale(RECT rect);

    int Downscale(int value);
    SIZE Downscale(SIZE size);

    int Rescale(int value, double scaleFactorAjustment);
    void AjustScaleFactor(double scaleFactorAjustment);

    HFONT CreateCustomFont(HWND hWindow, FontSizes fontSize, bool isItalic = false, bool isUnderscored = false);
    HBRUSH CreateCustomBrush(Colors color);
    SIZE GetTextSize(HDC hdc, const wchar_t* text, HFONT font);
    TEXTMETRIC GetFontMetrics(HDC hdc, HFONT font);

    HDC CreateInMemoryDC(DWORD width, DWORD height);
    void ResizeDC(HDC& hdc, DWORD width, DWORD height);
    DWORD CopyDC(HDC source, HDC target, DWORD width, DWORD height);
    void ClearDC(HDC hdc, DWORD width, DWORD height);
};