#pragma once
#include "Infrastructure\ErrorHandling\ErrorHandler.h"
#include "Infrastructure\ServiceRegistry\ServiceRegistry.h"
#include "Presentation\Framework\Providers\ScaleProvider.h"
#include "Presentation\Framework\Providers\ScrollProvider.h"
#include "Presentation\MessageBus.h"

class ScrollProvider;

class ViewContext
{
private:
    ServiceRegistry* serviceRegistry;

public:
    ViewContext(ServiceRegistry* registry);

    ErrorHandler* GetErrorHandler() const;
    ServiceRegistry* GetServiceRegistry() const;

    ScaleProvider* GetScaleProvider() const;
    ScrollProvider* GetScrollProvider() const;
    RenderingProvider* GetRenderingProvider() const;
    RenderingContext* GetRenderingContext() const;
    DeviceContextProvider* GetDeviceContextProvider() const;
    MessageBus* GetMessageBus() const;
    Logger* GetLogger() const;
};
