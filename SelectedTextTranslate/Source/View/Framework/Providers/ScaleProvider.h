#pragma once
#include "View\Framework\Dto\Point\Point.h"
#include "View\Framework\Dto\Point\PointReal.h"
#include "View\Framework\Dto\Size\Size.h"
#include "View\Framework\Dto\Size\SizeReal.h"
#include "View\Framework\Dto\Rect\Rect.h"
#include "View\Framework\Dto\Rect\RectReal.h"


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

    Rect Scale(RectReal rect) const;
    Size Scale(SizeReal size) const;
    Point Scale(PointReal point) const;
    Size Scale(Size size) const;
    Point Scale(Point point) const;

    double Downscale(double value) const;
    double Downscale(int value) const;
    SizeReal Downscale(Size size) const;
    PointReal Downscale(Point size) const;
    RectReal Downscale(Rect rect) const;

    int Rescale(int value, double scaleFactorAdjustment) const;
    void AdjustScaleFactor(double scaleFactorAdjustment);
    bool IsScalingAllowed(double scaleFactorAdjustment) const;
};
