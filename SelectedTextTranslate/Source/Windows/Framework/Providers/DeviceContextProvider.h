#pragma once
class DeviceContextProvider
{
public:
    HDC CreateInMemoryDC(Size dcSize) const;
    void ResizeDC(HDC& hdc, Size newDcSize) const;
    DWORD CopyDC(HDC source, HDC target, Size dcSize) const;

    DeviceContextProvider();
    ~DeviceContextProvider();
};