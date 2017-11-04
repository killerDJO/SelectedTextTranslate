#include "Presentation\Framework\Dto\ViewStateDescriptor.h"
ViewStateDescriptor::ViewStateDescriptor( DeviceContextProvider* deviceContextProvider)
{
    this->deviceContextProvider = deviceContextProvider;
    deviceContextBuffer = nullptr;
    isVisible = false;
    viewState = ViewStates::New;
}

void ViewStateDescriptor::SetLayout(LayoutDescriptor layout)
{
    this->layout = layout;
    ResetToLayout();
    deviceContextBuffer = new DeviceContextBuffer(deviceContextProvider, layout.GetSize());
}

LayoutDescriptor ViewStateDescriptor::GetLayout() const
{
    return layout;
}

void ViewStateDescriptor::ResetToLayout()
{
    windowSize = layout.GetSize();
    position = layout.GetPosition();
    contentSize = Size(0, 0);
}

void ViewStateDescriptor::UpdateContent(Renderer* renderer)
{
    Size renderedSize = renderer->GetSize();
    Size allowedRenderedSize = Size(
        layout.GetOverflowX() == OverflowModes::Fixed ? layout.GetSize().GetWidth() : renderedSize.GetWidth(),
        layout.GetOverflowY() == OverflowModes::Fixed ? layout.GetSize().GetHeight() : renderedSize.GetHeight()
    );

    if (!allowedRenderedSize.Equals(deviceContextBuffer->GetSize()))
    {
        deviceContextBuffer->Resize(allowedRenderedSize);
    }

    renderer->Render(deviceContextBuffer);

    StretchToSize(allowedRenderedSize);
}

DeviceContextBuffer* ViewStateDescriptor::GetDeviceContextBuffer() const
{
    return deviceContextBuffer;
}

void ViewStateDescriptor::StretchToSize(Size size)
{
    if (layout.GetOverflowX() == OverflowModes::Stretch && size.GetWidth() > windowSize.GetWidth())
    {
        windowSize = Size(size.GetWidth(), windowSize.GetHeight());
    }

    if (layout.GetOverflowY() == OverflowModes::Stretch && size.GetHeight() > windowSize.GetHeight())
    {
        windowSize = Size(windowSize.GetWidth(), size.GetHeight());
    }
}

Size ViewStateDescriptor::GetWindowSize() const
{
    return windowSize;
}

Size ViewStateDescriptor::GetContentSize() const
{
    return contentSize;
}

void ViewStateDescriptor::SetContentSize(Size contentSize)
{
    StretchToSize(contentSize);
    this->contentSize = contentSize;
}

Point ViewStateDescriptor::GetPosition() const
{
    return position;
}

void ViewStateDescriptor::SetPosition(Point position)
{
    this->position = position;
}

ViewStates ViewStateDescriptor::GetViewState() const
{
    return viewState;
}

void ViewStateDescriptor::SetViewState(ViewStates viewState)
{
    this->viewState = viewState;
}

Rect ViewStateDescriptor::GetBoundingRect() const
{
    return Rect(position, windowSize);
}

void ViewStateDescriptor::MakeVisible()
{
    isVisible = true;
}

void ViewStateDescriptor::MakeHidden()
{
    isVisible = false;
}

bool ViewStateDescriptor::IsVisible() const
{
    return isVisible;
}

ViewStateDescriptor::~ViewStateDescriptor()
{
    delete deviceContextBuffer;
}