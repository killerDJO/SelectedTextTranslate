#include "Presentation\Framework\Providers\DeviceContextProvider.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

HDC DeviceContextProvider::CreateDeviceContext(Size deviceContextSize) const
{
    HDC hdc = CreateCompatibleDC(nullptr);
    AssertCriticalWinApiResult(hdc);

    BITMAPINFO i;
    ZeroMemory(&i.bmiHeader, sizeof(BITMAPINFOHEADER));
    i.bmiHeader.biWidth = deviceContextSize.GetWidth();
    i.bmiHeader.biHeight = deviceContextSize.GetHeight();
    i.bmiHeader.biPlanes = 1;
    i.bmiHeader.biBitCount = 24;
    i.bmiHeader.biSizeImage = 0;
    i.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    i.bmiHeader.biClrUsed = 0;
    i.bmiHeader.biClrImportant = 0;
    VOID *pvBits;

    if (deviceContextSize.GetWidth() != 0 && deviceContextSize.GetHeight() != 0)
    {
        HBITMAP bitmap = CreateDIBSection(hdc, &i, DIB_RGB_COLORS, &pvBits, nullptr, 0);
        AssertCriticalWinApiResult(bitmap);

        AssertCriticalWinApiResult(SelectObject(hdc, bitmap));
        AssertCriticalWinApiResult(DeleteObject(bitmap));
    }

    return hdc;
}

void DeviceContextProvider::DeleteDeviceContext(HDC deviceContext) const
{
    AssertCriticalWinApiResult(DeleteDC(deviceContext));
}

void DeviceContextProvider::ResizeDeviceContext(HDC &deviceContext, Size newDeviceContextSize) const
{
    DeleteDeviceContext(deviceContext);
    deviceContext = CreateDeviceContext(newDeviceContextSize);
}

void DeviceContextProvider::ClearDeviceContext(HDC deviceContext, Size deviceContextSize, HBRUSH brush) const
{
    RECT rect;
    rect.top = 0;
    rect.left = 0;
    rect.bottom = deviceContextSize.GetHeight();
    rect.right = deviceContextSize.GetWidth();

    AssertCriticalWinApiResult(FillRect(deviceContext, &rect, brush));
}

void DeviceContextProvider::CopyDeviceContext(HDC source, HDC target, Size deviceContextSize) const
{
    AssertCriticalWinApiResult(BitBlt(target, 0, 0, deviceContextSize.GetWidth(), deviceContextSize.GetHeight(), source, 0, 0, SRCCOPY));
}
