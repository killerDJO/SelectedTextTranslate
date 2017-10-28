#include "Presentation\Framework\ViewContext.h"

ViewContext::ViewContext(ServiceRegistry* registry)
{
    this->serviceRegistry = registry;
}

ErrorHandler* ViewContext::GetErrorHandler() const
{
    return serviceRegistry->Get<ErrorHandler>();
}

ServiceRegistry* ViewContext::GetServiceRegistry() const
{
    return serviceRegistry;
}

ScaleProvider* ViewContext::GetScaleProvider() const
{
    return serviceRegistry->Get<ScaleProvider>();
}

ScrollProvider* ViewContext::GetScrollProvider() const
{
    return serviceRegistry->Get<ScrollProvider>();
}

RenderingProvider* ViewContext::GetRenderingProvider() const
{
    return serviceRegistry->Get<RenderingProvider>();
}

RenderingContext* ViewContext::GetRenderingContext() const
{
    return serviceRegistry->Get<RenderingContext>();
}

DeviceContextProvider* ViewContext::GetDeviceContextProvider() const
{
    return serviceRegistry->Get<DeviceContextProvider>();
}

MessageBus* ViewContext::GetMessageBus() const
{
    return serviceRegistry->Get<MessageBus>();
}

Logger* ViewContext::GetLogger() const
{
    return serviceRegistry->Get<Logger>();
}
