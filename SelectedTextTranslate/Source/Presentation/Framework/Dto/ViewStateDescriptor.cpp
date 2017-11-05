#include "Presentation\Framework\Dto\ViewStateDescriptor.h"
ViewStateDescriptor::ViewStateDescriptor( DeviceContextProvider* deviceContextProvider)
{
    this->deviceContextProvider = deviceContextProvider;
    deviceContextBuffer = nullptr;
    isVisible = false;
    viewStatus = ViewStatus::New;
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
    AssertLayoutIsSet();

    viewSize = layout.GetSize();
    position = layout.GetPosition();
    contentSize = Size(0, 0);
}

void ViewStateDescriptor::UpdateDeviceContext(Renderer* renderer)
{
    AssertLayoutIsSet();

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
}

DeviceContextBuffer* ViewStateDescriptor::GetDeviceContextBuffer() const
{
    AssertLayoutIsSet();

    return deviceContextBuffer;
}

void ViewStateDescriptor::StretchToSize(Size size)
{
    if (layout.GetOverflowX() == OverflowModes::Stretch && size.GetWidth() > viewSize.GetWidth())
    {
        viewSize = Size(size.GetWidth(), viewSize.GetHeight());
    }

    if (layout.GetOverflowY() == OverflowModes::Stretch && size.GetHeight() > viewSize.GetHeight())
    {
        viewSize = Size(viewSize.GetWidth(), size.GetHeight());
    }
}

Size ViewStateDescriptor::GetViewSize() const
{
    return viewSize;
}

Size ViewStateDescriptor::GetContentSize() const
{
    return contentSize;
}

void ViewStateDescriptor::SetContentSize(Size newContentSize)
{
    AssertLayoutIsSet();

    contentSize = newContentSize;
    StretchToSize(contentSize);
}

Point ViewStateDescriptor::GetPosition() const
{
    return position;
}

void ViewStateDescriptor::SetPosition(Point position)
{
    this->position = position;
}

ViewStatus ViewStateDescriptor::GetViewStatus() const
{
    return viewStatus;
}

void ViewStateDescriptor::SetViewStatus(ViewStatus viewStatus)
{
    this->viewStatus = viewStatus;
}

Rect ViewStateDescriptor::GetBoundingRect() const
{
    return Rect(position, viewSize);
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

void ViewStateDescriptor::AssertLayoutIsSet() const
{
    if (layout.IsEmpty())
    {
        throw SelectedTextTranslateFatalException(L"Layout is not set.");
    }
}

ViewStateDescriptor::~ViewStateDescriptor()
{
    delete deviceContextBuffer;
}