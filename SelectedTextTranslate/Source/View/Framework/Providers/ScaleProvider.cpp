#include "View\Framework\Providers\ScaleProvider.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateException.h"

ScaleProvider::ScaleProvider()
{
    ComputeScaleFactor();
}

void ScaleProvider::ComputeScaleFactor()
{
    RECT workarea;
    AssertCriticalWinApiResult(SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0));

    UINT screenResolutionY = workarea.bottom - workarea.top;
    scaleFactor = screenResolutionY / 860.0;
}

int ScaleProvider::Scale(int value) const
{
    return Scale((double)value);
}

int ScaleProvider::Scale(double value) const
{
    return roundToInt(value * scaleFactor);
}

Size ScaleProvider::Scale(Size size) const
{
    return Size(Scale(size.GetWidth()), Scale(size.GetHeight()));
}

Point ScaleProvider::Scale(Point point) const
{
    return Point(Scale(point.GetX()), Scale(point.GetY()));
}

double ScaleProvider::Downscale(int value) const
{
    return Downscale((double)value);
}

double ScaleProvider::Downscale(double value) const
{
    return value / scaleFactor;
}

int ScaleProvider::Rescale(int value, double scaleFactorAdjustment) const
{
    return roundToInt(Downscale(value) * (scaleFactor + scaleFactorAdjustment));
}

void ScaleProvider::AdjustScaleFactor(double scaleFactorAdjustment)
{
    if (!IsScalingAllowed(scaleFactorAdjustment))
    {
        throw SelectedTextTranslateException(L"Scaling to this size is not allowed");
    }

    scaleFactor += scaleFactorAdjustment;
}

bool ScaleProvider::IsScalingAllowed(double scaleFactorAdjustment) const
{
    double newScaleFactor = scaleFactor + scaleFactorAdjustment;
    return newScaleFactor >= 0.9 && newScaleFactor <= 2;
}

ScaleProvider::~ScaleProvider()
{
}
