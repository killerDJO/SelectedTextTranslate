#include "Windows\Framework\WindowContext.h"

WindowContext::WindowContext(
    HINSTANCE hInstance,
    ScrollProvider* scrollProvider,
    ScaleProvider* scaleProvider,
    DeviceContextProvider* deviceContextProvider,
    HotkeyProvider* hotkeyProvider,
    RenderingContext* renderingContext)
{
    this->hInstance = hInstance;
    this->scrollProvider = scrollProvider;
    this->scaleProvider = scaleProvider;
    this->deviceContextProvider = deviceContextProvider;
    this->hotkeyProvider = hotkeyProvider;
    this->renderingContext = renderingContext;
}

HINSTANCE WindowContext::GetInstance() const
{
    return hInstance;
}

ScrollProvider* WindowContext::GetScrollProvider() const
{
    return scrollProvider;
}

ScaleProvider* WindowContext::GetScaleProvider() const
{
    return scaleProvider;
}

DeviceContextProvider* WindowContext::GetDeviceContextProvider() const
{
    return deviceContextProvider;
}

HotkeyProvider* WindowContext::GetHotkeyProvider() const
{
    return hotkeyProvider;
}

RenderingContext* WindowContext::GetRenderingContext() const
{
    return renderingContext;
}

WindowContext::~WindowContext()
{
}
