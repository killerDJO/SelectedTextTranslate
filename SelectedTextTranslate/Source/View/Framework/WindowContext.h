#pragma once
#include "View\Framework\Providers\ScrollProvider.h"
#include "View\Framework\Providers\ScaleProvider.h"
#include "View\Framework\Providers\DeviceContextProvider.h"
#include "View\Framework\RenderingContext.h"
#include "Logging\Logger.h"

class ScrollProvider;
class RenderingContext;

class WindowContext
{
    HINSTANCE hInstance;

    ScrollProvider* scrollProvider;
    ScaleProvider* scaleProvider;
    DeviceContextProvider* deviceContextProvider;
    RenderingContext* renderingContext;
    Logger* logger;

public:
    WindowContext(
        HINSTANCE hInstance,
        ScrollProvider* scrollProvider,
        ScaleProvider* scaleProvider,
        DeviceContextProvider* deviceContextProvider,
        RenderingContext* renderingContext,
        Logger* logger);
    ~WindowContext();

    HINSTANCE GetInstance() const;
    ScrollProvider* GetScrollProvider() const;
    ScaleProvider* GetScaleProvider() const;
    DeviceContextProvider* GetDeviceContextProvider() const;
    RenderingContext* GetRenderingContext() const;
    Logger* GetLogger() const;
};