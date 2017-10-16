#include "View\Framework\View\ViewContext.h"

ViewContext::ViewContext(
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

HINSTANCE ViewContext::GetInstance() const
{
    return hInstance;
}

ScrollProvider* ViewContext::GetScrollProvider() const
{
    return scrollProvider;
}

ScaleProvider* ViewContext::GetScaleProvider() const
{
    return scaleProvider;
}

DeviceContextProvider* ViewContext::GetDeviceContextProvider() const
{
    return deviceContextProvider;
}

ErrorHandler* ViewContext::GetErrorHandler() const
{
    return errorHandler;
}

RenderingContext* ViewContext::GetRenderingContext() const
{
    return renderingContext;
}

RenderingProvider* ViewContext::GetRenderingProvider() const
{
    return renderingProvider;
}

Logger* ViewContext::GetLogger() const
{
    return logger;
}

MessageBus* ViewContext::GetMessageBus() const
{
    return messageBus;
}