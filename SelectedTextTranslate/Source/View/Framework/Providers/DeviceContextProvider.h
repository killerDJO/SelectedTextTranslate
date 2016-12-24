#pragma once
#include "View\Framework\Dto\Positioning\Size.h"

class DeviceContextProvider
{
public:
    HDC CreateDeviceContext(Size deviceContextSize) const;
    void DeleteDeviceContext(HDC deviceContext) const;

    void ResizeDeviceContext(HDC& deviceContext, Size newDeviceContextSize) const;

    void CopyDeviceContext(HDC source, HDC target, Size deviceContextSize) const;
};
