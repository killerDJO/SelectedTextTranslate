#include "Windows\Framework\Providers\DeviceContextProvider.h"

DeviceContextProvider::DeviceContextProvider()
{
}

HDC DeviceContextProvider::CreateInMemoryDC(Size dcSize) const
{
    HDC hdc = CreateCompatibleDC(nullptr);

    BITMAPINFO i;
    ZeroMemory(&i.bmiHeader, sizeof(BITMAPINFOHEADER));
    i.bmiHeader.biWidth = dcSize.Width;
    i.bmiHeader.biHeight = dcSize.Height;
    i.bmiHeader.biPlanes = 1;
    i.bmiHeader.biBitCount = 24;
    i.bmiHeader.biSizeImage = 0;
    i.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    i.bmiHeader.biClrUsed = 0;
    i.bmiHeader.biClrImportant = 0;
    VOID *pvBits;
    HBITMAP bitmap = CreateDIBSection(hdc, &i, DIB_RGB_COLORS, &pvBits, nullptr, 0);

    SelectObject(hdc, bitmap);

    return hdc;
}

void DeviceContextProvider::ResizeDC(HDC &hdc, Size dcSize) const
{
    DeleteDC(hdc);
    hdc = CreateInMemoryDC(dcSize);
}

DWORD DeviceContextProvider::CopyDC(HDC source, HDC target, Size dcSize) const
{
    return BitBlt(target, 0, 0, dcSize.Width, dcSize.Height, source, 0, 0, SRCCOPY);
}

DeviceContextProvider::~DeviceContextProvider()
{
}