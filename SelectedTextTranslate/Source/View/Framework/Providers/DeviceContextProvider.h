#pragma once
#include "View\Framework\Dto\Positioning\Size.h"

class DeviceContextProvider
{
public:
    DeviceContextProvider();

    HDC CreateDeviceContext(Size deviceContextSize) const;

    void ResizeDeviceContext(HDC& deviceContext, Size newDeviceContextSize) const;

    void CopyDeviceContext(HDC source, HDC target, Size deviceContextSize) const;
};
