#pragma once

class DeviceContextProvider
{
public:
    DeviceContextProvider();
    ~DeviceContextProvider();

    HDC CreateDeviceContext(Size deviceContextSize) const;

    void ResizeDeviceContext(HDC& deviceContext, Size newDeviceContextSize) const;

    void CopyDeviceContext(HDC source, HDC target, Size deviceContextSize) const;
};