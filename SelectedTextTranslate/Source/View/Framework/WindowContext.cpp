#include "View\Framework\WindowContext.h"

WindowContext::WindowContext(
    HINSTANCE hInstance,
    ScrollProvider* scrollProvider,
    ScaleProvider* scaleProvider,
    DeviceContextProvider* deviceContextProvider,
    MessageBus* messageBus,
    ErrorHandler* errorHandler,
    RenderingContext* renderingContext,
    RenderingProvider* renderingProvider,
    Logger* logger)
{
    this->hInstance = hInstance;
    this->scrollProvider = scrollProvider;
    this->scaleProvider = scaleProvider;
    this->deviceContextProvider = deviceContextProvider;
    this->renderingContext = renderingContext;
    this->renderingProvider = renderingProvider;
    this->errorHandler = errorHandler;
    this->logger = logger;
    this->messageBus = messageBus;
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

ErrorHandler* WindowContext::GetErrorHandler() const
{
    return errorHandler;
}

RenderingContext* WindowContext::GetRenderingContext() const
{
    return renderingContext;
}

RenderingProvider* WindowContext::GetRenderingProvider() const
{
    return renderingProvider;
}

Logger* WindowContext::GetLogger() const
{
    return logger;
}

MessageBus* WindowContext::GetMessageBus() const
{
    return messageBus;
}