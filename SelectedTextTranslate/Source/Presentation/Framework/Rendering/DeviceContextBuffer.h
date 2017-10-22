#pragma once
#include "Presentation\Framework\Providers\DeviceContextProvider.h"
#include "Presentation\Framework\Dto\Positioning\Size.h"

class DeviceContextBuffer
{
    HDC bufferingDeviceContext;
    Size bufferingDeviceContextSize;

    DeviceContextProvider* deviceContextProvider;

public:
    DeviceContextBuffer(DeviceContextProvider* deviceContextProvider, Size deviceContextSize);
    ~DeviceContextBuffer();

    Size GetSize() const;
    HDC GetDeviceContext() const;

    void Resize(Size newDeviceContextSize);
    void Render(HDC deviceContext, Size sizeToRender) const;
};