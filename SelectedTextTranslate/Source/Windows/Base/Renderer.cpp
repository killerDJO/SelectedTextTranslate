#include "Windows\Base\Renderer.h"

Renderer::Renderer()
{
    ComputeAjustmentParameters();
}

void Renderer::ComputeAjustmentParameters()
{
    RECT workarea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);

    UINT screenResolutionX = workarea.right - workarea.left;
    UINT screenResolutionY = workarea.bottom - workarea.top;
    kX = screenResolutionX / 1440.0;
    kY = screenResolutionY / 860.0;
}

DWORD Renderer::AdjustToXResolution(double value)
{
    return roundToInt(value * kX);
}

DWORD Renderer::AdjustToYResolution(double value)
{
    return roundToInt(value * kY);
}

SIZE Renderer::GetTextSize(HDC hdc, const wchar_t* text, HFONT font)
{
    SelectObject(hdc, font);

    SIZE textSize;
    GetTextExtentPoint32(hdc, text, wcslen(text), &textSize);

    return textSize;
}

POINT Renderer::PrintText(HDC hdc, const wchar_t* text, HFONT font, COLORREF color, int x, int y, PPOINT bottomRight)
{
    SelectObject(hdc, font);
    SetTextColor(hdc, color);

    SIZE textSize;
    GetTextExtentPoint32(hdc, text, wcslen(text), &textSize);

    TextOut(hdc, x, y, text, _tcslen(text));

    bottomRight->x = max(bottomRight->x, x + textSize.cx);
    bottomRight->y = max(bottomRight->y, y + textSize.cy);

    POINT result;
    result.x = x + textSize.cx;
    result.y = y + textSize.cy;
    return result;
}

void Renderer::DrawRect(HDC hdc, RECT rect, HBRUSH brush, PPOINT bottomRight)
{
    FillRect(hdc, &rect, brush);

    bottomRight->x = max(bottomRight->x, rect.right);
    bottomRight->y = max(bottomRight->y, rect.bottom);
}

HDC Renderer::CreateInMemoryDC(DWORD width, DWORD height)
{
    HDC hdc = CreateCompatibleDC(NULL);

    BITMAPINFO i;
    ZeroMemory(&i.bmiHeader, sizeof(BITMAPINFOHEADER));
    i.bmiHeader.biWidth = width;
    i.bmiHeader.biHeight = height;
    i.bmiHeader.biPlanes = 1;
    i.bmiHeader.biBitCount = 24;
    i.bmiHeader.biSizeImage = 0;
    i.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    i.bmiHeader.biClrUsed = 0;
    i.bmiHeader.biClrImportant = 0;
    VOID *pvBits;
    HBITMAP bitmap = CreateDIBSection(hdc, &i, DIB_RGB_COLORS, &pvBits, NULL, 0);

    SelectObject(hdc, bitmap);

    return hdc;
}

void Renderer::ResizeDC(HDC &hdc, DWORD width, DWORD height)
{
    DeleteDC(hdc);
    hdc = CreateInMemoryDC(width, height);
}

void Renderer::ClearDC(HDC hdc, DWORD width, DWORD height)
{
    RECT rect;
    rect.top = 0;
    rect.left = 0;
    rect.bottom = height;
    rect.right = width;
    FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
}

DWORD Renderer::CopyDC(HDC source, HDC target, DWORD width, DWORD height)
{
    return BitBlt(target, 0, 0, width, height, source, 0, 0, SRCCOPY);
}

Renderer::~Renderer()
{
}