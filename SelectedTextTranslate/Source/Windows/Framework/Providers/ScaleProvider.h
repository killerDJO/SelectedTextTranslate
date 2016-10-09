#pragma once
#include "Windows\Framework\Dto\WindowDescriptor.h"

/// <summary>
/// Provides methods to work with scaling.
/// </summary>
class ScaleProvider
{
private:
    /// <summary>
    /// The current scale factor.
    /// </summary>
    double scaleFactor;

    /// <summary>
    /// Computes the initial scale factor.
    /// </summary>
    void ComputeScaleFactor();

public:
    /// <summary>
    /// Initializes a new instance of the <see cref="ScaleProvider"/> class.
    /// </summary>
    ScaleProvider();
    /// <summary>
    /// Finalizes an instance of the <see cref="ScaleProvider"/> class.
    /// </summary>
    ~ScaleProvider();

    /// <summary>
    /// Scales the specified value.
    /// </summary>
    /// <param name="value">The value.</param>
    /// <returns>The scaled value.</returns>
    int Scale(int value) const;

    /// <summary>
    /// Scales the specified window descriptor.
    /// </summary>
    /// <param name="windowDescriptor">The window descriptor.</param>
    /// <returns>The scaled window descriptor.</returns>
    WindowDescriptor Scale(WindowDescriptor windowDescriptor) const;

    /// <summary>
    /// Scales the specified rectangle.
    /// </summary>
    /// <param name="rect">The rectangle.</param>
    /// <returns>The scaled rectangle.</returns>
    Rect Scale(Rect rect) const;

    /// <summary>
    /// Scales the specified size.
    /// </summary>
    /// <param name="size">The size.</param>
    /// <returns>The scaled size.</returns>
    Size Scale(Size size) const;

    /// <summary>
    /// Scales the specified point.
    /// </summary>
    /// <param name="point">The point.</param>
    /// <returns>The scaled point.</returns>
    Point Scale(Point point) const;

    /// <summary>
    /// Downscales the specified value.
    /// </summary>
    /// <param name="value">The value.</param>
    /// <returns>The downscaled value.</returns>
    int Downscale(int value) const;

    /// <summary>
    /// Downscales the specified size.
    /// </summary>
    /// <param name="size">The size.</param>
    /// <returns>The scaled size.</returns>
    Size Downscale(Size size) const;

    /// <summary>
    /// Rescales the specified scaled with current scale factor value to the new scaled factor.
    /// </summary>
    /// <param name="value">The value to rescale.</param>
    /// <param name="scaleFactorAdjustment">The scale factor to add to current scale factor.</param>
    /// <returns></returns>
    int Rescale(int value, double scaleFactorAdjustment) const;

    /// <summary>
    /// Adjusts the scale factor.
    /// </summary>
    /// <param name="scaleFactorAjustment">The scale factor to add to current scale factor.</param>
    void AdjustScaleFactor(double scaleFactorAdjustment);
};