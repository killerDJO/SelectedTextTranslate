#pragma once

/// <summary>
/// Represent window states.
/// </summary>
enum class WindowStates
{
    /// <summary>
    /// The new state.
    /// </summary>
    New = 0,

    /// <summary>
    /// The window has been initialized.
    /// </summary>
    Initialized = 1,

    /// <summary>
    /// The window has been rendered.
    /// </summary>
    Rendered = 2,

    /// <summary>
    /// The window has been drawn after render.
    /// </summary>
    Drawn = 3,

    /// <summary>
    /// The window has been destroyed.
    /// </summary>
    Destroyed = 4,
};