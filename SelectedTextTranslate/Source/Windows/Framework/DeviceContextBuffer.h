#pragma once
#include "Windows\Framework\Providers\DeviceContextProvider.h"

/// <summary>
/// The device context buffer. Used to store rendered window content.
/// </summary>
class DeviceContextBuffer
{
    /// <summary>
    /// The buffering device context.
    /// </summary>
    HDC bufferingDeviceContext;

    /// <summary>
    /// The buffering device context size.
    /// </summary>
    Size bufferingDeviceContextSize;

    /// <summary>
    /// The device context provider.
    /// </summary>
    DeviceContextProvider* deviceContextProvider;

public:
    /// <summary>
    /// Initializes a new instance of the <see cref="DeviceContextBuffer"/> class.
    /// </summary>
    /// <param name="deviceContextProvider">The device context provider.</param>
    /// <param name="deviceContextSize">Size of the device context.</param>
    DeviceContextBuffer(DeviceContextProvider* deviceContextProvider, Size deviceContextSize);

    /// <summary>
    /// Finalizes an instance of the <see cref="DeviceContextBuffer"/> class.
    /// </summary>
    ~DeviceContextBuffer();

    /// <summary>
    /// Gets the size.
    /// </summary>
    /// <returns>The size of the buffer.</returns>
    Size GetSize() const;

    /// <summary>
    /// Gets the device context.
    /// </summary>
    /// <returns>The buffering device context.</returns>
    HDC GetDeviceContext() const;

    /// <summary>
    /// Resizes the buffering device context.
    /// </summary>
    /// <param name="newDeviceContextSize">New size of the device context.</param>
    void Resize(Size newDeviceContextSize);

    /// <summary>
    /// Renders the buffer onto specified device context using provided size.
    /// </summary>
    /// <param name="deviceContext">The device context to render onto.</param>
    /// <param name="sizeToRender">The size to render.</param>
    void Render(HDC deviceContext, Size sizeToRender) const;
};