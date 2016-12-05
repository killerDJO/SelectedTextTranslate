#pragma once
#include "View\Framework\Dto\WindowDescriptor.h"

class ScaleProvider
{
private:
    double scaleFactor;

    void ComputeScaleFactor();

public:
    ScaleProvider();
    ~ScaleProvider();

    int Scale(int value) const;
    WindowDescriptor Scale(WindowDescriptor windowDescriptor) const;
    Rect Scale(Rect rect) const;
    Size Scale(Size size) const;
    Point Scale(Point point) const;

    int Downscale(int value) const;
    Size Downscale(Size size) const;

    int Rescale(int value, double scaleFactorAdjustment) const;
    void AdjustScaleFactor(double scaleFactorAdjustment);
    bool IsScalingAllowed(double scaleFactorAdjustment) const;
};