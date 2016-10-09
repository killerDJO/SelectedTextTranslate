#pragma once
#include "Windows\Framework\Providers\ScrollProvider.h"
#include "Windows\Framework\Providers\ScaleProvider.h"
#include "Windows\Framework\Providers\DeviceContextProvider.h"
#include "Windows\Framework\RenderingContext.h"

class ScrollProvider;
class RenderingContext;

/// <summary>
/// The context which holds other context and providers.
/// </summary>
class WindowContext
{
    /// <summary>
    /// The current application instance.
    /// </summary>
    HINSTANCE hInstance;

    /// <summary>
    /// The scroll provider.
    /// </summary>
    ScrollProvider* scrollProvider;

    /// <summary>
    /// The scale provider.
    /// </summary>
    ScaleProvider* scaleProvider;

    /// <summary>
    /// The device context provider.
    /// </summary>
    DeviceContextProvider* deviceContextProvider;

    /// <summary>
    /// The rendering context.
    /// </summary>
    RenderingContext* renderingContext;

public:
    /// <summary>
    /// Initializes a new instance of the <see cref="WindowContext"/> class.
    /// </summary>
    /// <param name="hInstance">The h instance.</param>
    /// <param name="scrollProvider">The scroll provider.</param>
    /// <param name="scaleProvider">The scale provider.</param>
    /// <param name="deviceContextProvider">The device context provider.</param>
    /// <param name="HotkeyProvider">The hotkeys provider.</param>
    /// <param name="renderingContext">The rendering context.</param>
    WindowContext(
        HINSTANCE hInstance,
        ScrollProvider* scrollProvider,
        ScaleProvider* scaleProvider,
        DeviceContextProvider* deviceContextProvider,
        RenderingContext* renderingContext);

    /// <summary>
    /// Finalizes an instance of the <see cref="WindowContext"/> class.
    /// </summary>
    ~WindowContext();

    /// <summary>
    /// Gets the application instance.
    /// </summary>
    /// <returns>The application instance.</returns>
    HINSTANCE GetInstance() const;

    /// <summary>
    /// Gets the scroll provider.
    /// </summary>
    /// <returns>The scroll provider.</returns>
    ScrollProvider* GetScrollProvider() const;

    /// <summary>
    /// Gets the scale provider.
    /// </summary>
    /// <returns>The scale provider.</returns>
    ScaleProvider* GetScaleProvider() const;

    /// <summary>
    /// Gets the device context provider.
    /// </summary>
    /// <returns>The device context provider.</returns>
    DeviceContextProvider* GetDeviceContextProvider() const;

    /// <summary>
    /// Gets the rendering context.
    /// </summary>
    /// <returns>The rendering context.</returns>
    RenderingContext* GetRenderingContext() const;
};