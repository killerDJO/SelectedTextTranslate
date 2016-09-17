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

    int Scale(int x);
    WindowDescriptor Scale(WindowDescriptor windowDescriptor);
    RECT Scale(RECT rect);

    int Downscale(int x);
    SIZE Downscale(SIZE size);

    HFONT CreateCustomFont(HWND hWindow, FontSizes fontSize, bool isItalic = false, bool isUnderscored = false);
    HBRUSH CreateCustomBrush(Colors color);
    SIZE GetTextSize(HDC hdc, const wchar_t* text, HFONT font);

    HDC CreateInMemoryDC(DWORD width, DWORD height);
    void ResizeDC(HDC& hdc, DWORD width, DWORD height);
    DWORD CopyDC(HDC source, HDC target, DWORD width, DWORD height);
    void ClearDC(HDC hdc, DWORD width, DWORD height);
};