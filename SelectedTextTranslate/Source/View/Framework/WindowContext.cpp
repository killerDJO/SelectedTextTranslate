#include "View\Framework\WindowContext.h"

WindowContext::WindowContext(
    HINSTANCE hInstance,
    ScrollProvider* scrollProvider,
    ScaleProvider* scaleProvider,
    DeviceContextProvider* deviceContextProvider,
    ErrorStateProvider* errorStateProvider,
    RenderingContext* renderingContext,
    Logger* logger)
{
    this->hInstance = hInstance;
    this->scrollProvider = scrollProvider;
    this->scaleProvider = scaleProvider;
    this->deviceContextProvider = deviceContextProvider;
    this->renderingContext = renderingContext;
    this->errorStateProvider = errorStateProvider;
    this->logger = logger;
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

ErrorStateProvider* WindowContext::GetErrorStateProvider() const
{
    return errorStateProvider;
}

RenderingContext* WindowContext::GetRenderingContext() const
{
    return renderingContext;
}

Logger* WindowContext::GetLogger() const
{
    return logger;
}

WindowContext::~WindowContext()
{
}
