#pragma once
#include "Presentation\Components\Main\Dto\ViewDescriptor.h"
#include "Presentation\Components\Main\Enums\ApplicationViews.h"
#include "Presentation\Framework\Providers\ScaleProvider.h"

class MainViewModel
{
private:
    ApplicationViews applicationView;
    map<ApplicationViews, ViewDescriptor> applicationViewDescriptors;
    Size minSize;

    ViewDescriptor ScaleViewDescriptor(ViewDescriptor viewDescriptor, double scaleFactorAdjustment, ScaleProvider* scaleProvider) const;

public:
    MainViewModel(ApplicationViews applicationView, LayoutDescriptor layout);

    ApplicationViews GetApplicationView() const;
    void SetApplicationView(ApplicationViews applicationView);

    ViewDescriptor GetViewDescriptor();
    LayoutDescriptor GetLayoutDescriptor(ApplicationViews applicationView);
    LayoutDescriptor GetCurrentLayoutDescriptor();

    Size GetMinimumSize() const;

    void Scale(double scaleFactorAdjustment, ScaleProvider* scaleProvider);
};
