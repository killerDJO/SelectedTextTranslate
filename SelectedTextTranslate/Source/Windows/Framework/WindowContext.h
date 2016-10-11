#pragma once
#include "Windows\Framework\Providers\ScrollProvider.h"
#include "Windows\Framework\Providers\ScaleProvider.h"
#include "Windows\Framework\Providers\DeviceContextProvider.h"
#include "Windows\Framework\RenderingContext.h"

class ScrollProvider;
class RenderingContext;

class WindowContext
{
    HINSTANCE hInstance;

    ScrollProvider* scrollProvider;
    ScaleProvider* scaleProvider;
    DeviceContextProvider* deviceContextProvider;
    RenderingContext* renderingContext;

public:
    WindowContext(
        HINSTANCE hInstance,
        ScrollProvider* scrollProvider,
        ScaleProvider* scaleProvider,
        DeviceContextProvider* deviceContextProvider,
        RenderingContext* renderingContext);
    ~WindowContext();

    HINSTANCE GetInstance() const;
    ScrollProvider* GetScrollProvider() const;
    ScaleProvider* GetScaleProvider() const;
    DeviceContextProvider* GetDeviceContextProvider() const;
    RenderingContext* GetRenderingContext() const;
};