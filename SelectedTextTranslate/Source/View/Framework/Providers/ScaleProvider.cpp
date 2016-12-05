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
    return roundToInt(value * scaleFactor);
}

Rect ScaleProvider::Scale(Rect rect) const
{
    Rect scaledRect;
    scaledRect.X = Scale(rect.X);
    scaledRect.Y = Scale(rect.Y);
    scaledRect.Width = Scale(rect.Width);
    scaledRect.Height = Scale(rect.Height);
    return scaledRect;
}

Size ScaleProvider::Scale(Size size) const
{
    return Size(Scale(size.Width), Scale(size.Height));
}

Point ScaleProvider::Scale(Point point) const
{
    return Point(Scale(point.X), Scale(point.Y));
}

WindowDescriptor ScaleProvider::Scale(WindowDescriptor windowDescriptor) const
{
    return WindowDescriptor::CreateWindowDescriptor(
        Scale(windowDescriptor.GetPosition()),
        Scale(windowDescriptor.GetWindowSize()),
        windowDescriptor.GetOverflowX(),
        windowDescriptor.GetOverflowY(),
        windowDescriptor.IsAutoScaleEnabled(),
        windowDescriptor.GetName()
    );
}

int ScaleProvider::Downscale(int value) const
{
    return roundToInt(value / scaleFactor);
}

Size ScaleProvider::Downscale(Size size) const
{
    Size downscaledSize;
    downscaledSize.Width = Downscale(size.Width);
    downscaledSize.Height = Downscale(size.Height);
    return downscaledSize;
}

int ScaleProvider::Rescale(int value, double scaleFactorAdjustment) const
{
    return roundToInt(Downscale(value) * (scaleFactor + scaleFactorAdjustment));
}

void ScaleProvider::AdjustScaleFactor(double scaleFactorAdjustment)
{
    if(!IsScalingAllowed(scaleFactorAdjustment))
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
