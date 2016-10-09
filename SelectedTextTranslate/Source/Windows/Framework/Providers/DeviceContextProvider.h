#pragma once

/// <summary>
/// Provides common methods to work with device context.
/// </summary>
class DeviceContextProvider
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="DeviceContextProvider"/> class.
    /// </summary>
    DeviceContextProvider();

    /// <summary>
    /// Finalizes an instance of the <see cref="DeviceContextProvider"/> class.
    /// </summary>
    ~DeviceContextProvider();

    /// <summary>
    /// Creates the device context.
    /// </summary>
    /// <param name="deviceContextSize">Size of the device context.</param>
    /// <returns>Created device context.</returns>
    HDC CreateDeviceContext(Size deviceContextSize) const;

    /// <summary>
    /// Resizes the device context.
    /// </summary>
    /// <param name="deviceContext">The device context to resize.</param>
    /// <param name="newDeviceContextSize">New size of the device context.</param>
    void ResizeDeviceContext(HDC& deviceContext, Size newDeviceContextSize) const;

    /// <summary>
    /// Copies the device context.
    /// </summary>
    /// <param name="source">The source device context.</param>
    /// <param name="target">The target device context.</param>
    /// <param name="deviceContextSize">Size of the device context to copy.</param>
    /// <returns>The copy result.</returns>
    DWORD CopyDeviceContext(HDC source, HDC target, Size deviceContextSize) const;
};