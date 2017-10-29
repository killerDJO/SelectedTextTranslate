#include "Presentation\Components\Main\MainViewModel.h"

MainViewModel::MainViewModel(ApplicationViews applicationView, LayoutDescriptor layout)
{
    this->applicationView = applicationView;

    applicationViewDescriptors[ApplicationViews::Settings] = ViewDescriptor(layout, false);
    applicationViewDescriptors[ApplicationViews::Dictionary] = ViewDescriptor(layout, true);
    applicationViewDescriptors[ApplicationViews::TranslateResult] = ViewDescriptor(layout, true);
    minSize = layout.GetSize();
}

ApplicationViews MainViewModel::GetApplicationView() const
{
    return applicationView;
}

void MainViewModel::SetApplicationView(ApplicationViews applicationView)
{
    this->applicationView = applicationView;
}

ViewDescriptor* MainViewModel::GetViewDescriptor()
{
    return &applicationViewDescriptors[applicationView];
}

LayoutDescriptor MainViewModel::GetLayoutDescriptor(ApplicationViews applicationView)
{
    return applicationViewDescriptors[applicationView].GetLayoutDescriptor();
}

LayoutDescriptor MainViewModel::GetCurrentLayoutDescriptor()
{
    return applicationViewDescriptors[applicationView].GetLayoutDescriptor();;
}

Size MainViewModel::GetMinimumSize() const
{
    return minSize;
}

void MainViewModel::Scale(double scaleFactorAdjustment, ScaleProvider* scaleProvider)
{
    for(auto& applicationViewDescriptor : applicationViewDescriptors) {
        applicationViewDescriptor.second = ScaleViewDescriptor(applicationViewDescriptor.second, scaleFactorAdjustment, scaleProvider);
    }

    minSize = Size(
        scaleProvider->Rescale(minSize.GetWidth(), scaleFactorAdjustment),
        scaleProvider->Rescale(minSize.GetHeight(), scaleFactorAdjustment));
}

ViewDescriptor MainViewModel::ScaleViewDescriptor(ViewDescriptor viewDescriptor, double scaleFactorAdjustment, ScaleProvider* scaleProvider) const
{
    LayoutDescriptor layout = viewDescriptor.GetLayoutDescriptor();

    int scaledWidth = scaleProvider->Rescale(layout.GetSize().GetWidth(), scaleFactorAdjustment);
    int scaledHeight = scaleProvider->Rescale(layout.GetSize().GetHeight(), scaleFactorAdjustment);

    layout.SetPosition(Point(
        layout.GetPosition().GetX() - scaledWidth + layout.GetSize().GetWidth(),
        layout.GetPosition().GetY() - scaledHeight + layout.GetSize().GetHeight()));

    layout.SetSize(Size(scaledWidth, scaledHeight));

    return ViewDescriptor(layout, viewDescriptor.IsResizeable());
}
