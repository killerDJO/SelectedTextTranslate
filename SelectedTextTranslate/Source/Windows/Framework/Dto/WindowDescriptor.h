#pragma once
#include "Windows\Framework\Enums\OverflowModes.h"

/// <summary>
/// The structure which describes window parameters.
/// </summary>
struct WindowDescriptor
{
    /// <summary>
    /// The window initial position.
    /// </summary>
    Point Position;

    /// <summary>
    /// The window initial size.
    /// </summary>
    Size WindowSize;

    /// <summary>
    /// The vertical content overflow mode.
    /// </summary>
    OverflowModes OverflowY;

    /// <summary>
    /// The horizontal content overflow mode.
    /// </summary>
    OverflowModes OverflowX;

    /// <summary>
    /// Whether or not window position and size should be scaled before creating window.
    /// </summary>
    bool AutoScale;

    /// <summary>
    /// Creates the window descriptor.
    /// </summary>
    /// <param name="position">The initial position.</param>
    /// <param name="windowSize">The initial size of the window.</param>
    /// <param name="overflowX">The horizontal overflow.</param>
    /// <param name="overflowY">The vertical overflow.</param>
    /// <param name="autoScale">if set to <c>true</c> size and position will be scaled.</param>
    /// <returns>Created descriptor.</returns>
    static WindowDescriptor CreateWindowDescriptor(Point position, Size windowSize, OverflowModes overflowX, OverflowModes overflowY, bool autoScale = true);
    
    /// <summary>
    /// Creates the window descriptor with fixed overflow mode.
    /// </summary>
    /// <param name="position">The initial position.</param>
    /// <param name="windowSize">The initial size of the window.</param>
    /// <returns>Created descriptor.</returns>
    static WindowDescriptor CreateFixedWindowDescriptor(Point position, Size windowSize);

    /// <summary>
    /// Creates the window descriptor with stretch overflow mode.
    /// </summary>
    /// <param name="position">The initial position.</param>
    /// <returns>Created descriptor.</returns>
    static WindowDescriptor CreateStretchWindowDescriptor(Point position);

    /// <summary>
    /// Initializes a new instance of the <see cref="WindowDescriptor"/> struct.
    /// </summary>
    WindowDescriptor();

    /// <summary>
    /// Initializes a new instance of the <see cref="WindowDescriptor"/> struct.
    /// </summary>
    /// <param name="position">The initial position.</param>
    /// <param name="windowSize">The initial size of the window.</param>
    /// <param name="overflowX">The horizontal overflow.</param>
    /// <param name="overflowY">The vertical overflow.</param>
    /// <param name="autoScale">if set to <c>true</c> size and position will be scaled.</param>
    WindowDescriptor(Point position, Size windowSize, OverflowModes overflowX, OverflowModes overflowY, bool autoScale = true);
};