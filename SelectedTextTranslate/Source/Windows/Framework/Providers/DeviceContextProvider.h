#pragma once

class DeviceContextProvider
{
public:
    DeviceContextProvider();
    ~DeviceContextProvider();

    HDC CreateDeviceContext(Size deviceContextSize) const;

    void ResizeDeviceContext(HDC& deviceContext, Size newDeviceContextSize) const;

    DWORD CopyDeviceContext(HDC source, HDC target, Size deviceContextSize) const;
};