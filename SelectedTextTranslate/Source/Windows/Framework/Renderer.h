#pragma once

class Renderer
{
private:
    void ComputeAjustmentParameters();

public:
    double kX;
    double kY;

    Renderer();
    ~Renderer();

    DWORD AdjustToXResolution(double value);
    DWORD AdjustToYResolution(double value);

    SIZE GetTextSize(HDC hdc, const wchar_t* text, HFONT font);
    POINT PrintText(HDC hdc, const wchar_t* text, HFONT font, COLORREF color, int x, int y, PPOINT bottomRight);
    void DrawRect(HDC hdc, RECT rect, HBRUSH brush, PPOINT bottomRight);

    HDC CreateInMemoryDC(DWORD width, DWORD height);
    void ResizeDC(HDC& hdc, DWORD width, DWORD height);
    DWORD CopyDC(HDC source, HDC target, DWORD width, DWORD height);
    void ClearDC(HDC hdc, DWORD width, DWORD height);
};