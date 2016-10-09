#pragma once

/// <summary>
/// Represents different window content overflow modes.
/// </summary>
enum class OverflowModes
{
    /// <summary>
    /// Window's dimension will remain unchanged. Content will be clipped.
    /// </summary>
    Fixed = 1,

    /// <summary>
    /// Window's dimension will stretch to fit content.
    /// </summary>
    Stretch = 2,

    /// <summary>
    /// The corresponding scroll will appear.
    /// </summary>
    Scroll = 3
};