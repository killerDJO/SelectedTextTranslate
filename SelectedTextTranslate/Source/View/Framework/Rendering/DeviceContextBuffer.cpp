#include "View\Framework\Rendering\DeviceContextBuffer.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

DeviceContextBuffer::DeviceContextBuffer(DeviceContextProvider* deviceContextProvider, Size deviceContextSize)
{
    this->deviceContextProvider = deviceContextProvider;
    this->bufferingDeviceContextSize = deviceContextSize;
    this->bufferingDeviceContext = deviceContextProvider->CreateDeviceContext(bufferingDeviceContextSize);
}

Size DeviceContextBuffer::GetSize() const
{
    return bufferingDeviceContextSize;
}

HDC DeviceContextBuffer::GetDeviceContext() const
{
    return bufferingDeviceContext;
}

void DeviceContextBuffer::Resize(Size newDeviceContextSize)
{
    bufferingDeviceContextSize = newDeviceContextSize;
    deviceContextProvider->ResizeDeviceContext(bufferingDeviceContext, bufferingDeviceContextSize);
}

void DeviceContextBuffer::Render(HDC deviceContext, Size sizeToRender) const
{
    deviceContextProvider->CopyDeviceContext(bufferingDeviceContext, deviceContext, sizeToRender);
}

DeviceContextBuffer::~DeviceContextBuffer()
{
    AssertCriticalWinApiResult(DeleteDC(bufferingDeviceContext));
}