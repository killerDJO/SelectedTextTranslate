#pragma once
#include "Infrastructure\ErrorHandling\ErrorHandler.h"
#include "Infrastructure\Logging\Logger.h"
#include "View\Framework\Rendering\RenderingProvider.h"
#include "View\Framework\Providers\DeviceContextProvider.h"
#include "View\Framework\Providers\ScaleProvider.h"
#include "View\Framework\Providers\ScrollProvider.h"
#include "View\Framework\MessageBus.h"

class ScrollProvider;
class RenderingContext;

class ViewContext
{
    HINSTANCE hInstance;

    ScrollProvider* scrollProvider;
    ScaleProvider* scaleProvider;
    DeviceContextProvider* deviceContextProvider;
    MessageBus* messageBus;
    ErrorHandler* errorHandler;
    RenderingContext* renderingContext;
    RenderingProvider* renderingProvider;
    Logger* logger;

public:
    ViewContext(
        HINSTANCE hInstance,
        ScrollProvider* scrollProvider,
        ScaleProvider* scaleProvider,
        DeviceContextProvider* deviceContextProvider,
        MessageBus* messageBus,
        ErrorHandler* errorHandler,
        RenderingContext* renderingContext,
        RenderingProvider* renderingProvider,
        Logger* logger);

    HINSTANCE GetInstance() const;
    ScrollProvider* GetScrollProvider() const;
    ScaleProvider* GetScaleProvider() const;
    DeviceContextProvider* GetDeviceContextProvider() const;
    ErrorHandler* GetErrorHandler() const;
    RenderingContext* GetRenderingContext() const;
    RenderingProvider* GetRenderingProvider() const;
    Logger* GetLogger() const;
    MessageBus* GetMessageBus() const;
};