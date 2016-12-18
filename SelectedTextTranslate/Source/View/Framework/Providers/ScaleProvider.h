#pragma once
#include "View\Framework\Dto\Positioning\Point.h"
#include "View\Framework\Dto\Positioning\Size.h"

class ScaleProvider
{
private:
    double scaleFactor;

    void ComputeScaleFactor();

public:
    ScaleProvider();
    ~ScaleProvider();

    int Scale(double value) const;
    int Scale(int value) const;

    Size Scale(Size size) const;
    Point Scale(Point point) const;

    double Downscale(double value) const;
    double Downscale(int value) const;

    int Rescale(int value, double scaleFactorAdjustment) const;
    void AdjustScaleFactor(double scaleFactorAdjustment);
    bool IsScalingAllowed(double scaleFactorAdjustment) const;
};
