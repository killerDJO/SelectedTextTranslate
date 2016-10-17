#include "View\Framework\Providers\DeviceContextProvider.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

DeviceContextProvider::DeviceContextProvider()
{
}

HDC DeviceContextProvider::CreateDeviceContext(Size deviceContextSize) const
{
    HDC hdc = CreateCompatibleDC(nullptr);
    AssertCriticalWinApiResult(hdc);

    BITMAPINFO i;
    ZeroMemory(&i.bmiHeader, sizeof(BITMAPINFOHEADER));
    i.bmiHeader.biWidth = deviceContextSize.Width;
    i.bmiHeader.biHeight = deviceContextSize.Height;
    i.bmiHeader.biPlanes = 1;
    i.bmiHeader.biBitCount = 24;
    i.bmiHeader.biSizeImage = 0;
    i.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    i.bmiHeader.biClrUsed = 0;
    i.bmiHeader.biClrImportant = 0;
    VOID *pvBits;

    if(deviceContextSize.Width != 0 && deviceContextSize.Height != 0)
    {
        HBITMAP bitmap = CreateDIBSection(hdc, &i, DIB_RGB_COLORS, &pvBits, nullptr, 0);
        AssertCriticalWinApiResult(bitmap);

        AssertCriticalWinApiResult(SelectObject(hdc, bitmap));
        AssertCriticalWinApiResult(DeleteObject(bitmap));
    }

    return hdc;
}

void DeviceContextProvider::ResizeDeviceContext(HDC &deviceContext, Size newDeviceContextSize) const
{
    AssertCriticalWinApiResult(DeleteDC(deviceContext));
    deviceContext = CreateDeviceContext(newDeviceContextSize);
}

void DeviceContextProvider::CopyDeviceContext(HDC source, HDC target, Size deviceContextSize) const
{
    AssertCriticalWinApiResult(BitBlt(target, 0, 0, deviceContextSize.Width, deviceContextSize.Height, source, 0, 0, SRCCOPY));
}

DeviceContextProvider::~DeviceContextProvider()
{
}