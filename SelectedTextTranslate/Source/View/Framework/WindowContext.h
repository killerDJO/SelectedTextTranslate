#pragma once
#include "Infrastructure\ErrorHandling\ErrorHandler.h"
#include "Infrastructure\Logging\Logger.h"
#include "View\Framework\Providers\ScrollProvider.h"
#include "View\Framework\Providers\ScaleProvider.h"
#include "View\Framework\Providers\DeviceContextProvider.h"
#include "View\Framework\RenderingContext.h"
#include "View\Providers\HotkeyProvider.h"
#include "View\Framework\Providers\DialogsProvider.h"

class ScrollProvider;
class RenderingContext;

class WindowContext
{
    HINSTANCE hInstance;

    ScrollProvider* scrollProvider;
    ScaleProvider* scaleProvider;
    DeviceContextProvider* deviceContextProvider;
    DialogsProvider* dialogsProvider;
    ErrorHandler* errorHandler;
    RenderingContext* renderingContext;
    HotkeyProvider* hotkeyProvider;
    Logger* logger;

public:
    WindowContext(
        HINSTANCE hInstance,
        ScrollProvider* scrollProvider,
        ScaleProvider* scaleProvider,
        DeviceContextProvider* deviceContextProvider,
        DialogsProvider* dialogsProvider,
        HotkeyProvider* hotkeyProvider,
        ErrorHandler* errorHandler,
        RenderingContext* renderingContext,
        Logger* logger);
    ~WindowContext();

    HINSTANCE GetInstance() const;
    ScrollProvider* GetScrollProvider() const;
    ScaleProvider* GetScaleProvider() const;
    DeviceContextProvider* GetDeviceContextProvider() const;
    ErrorHandler* GetErrorHandler() const;
    RenderingContext* GetRenderingContext() const;
    Logger* GetLogger() const;
    HotkeyProvider* GetHotkeyProvider() const;
    DialogsProvider* GetDialogsProvider() const;
};