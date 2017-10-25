#include "Presentation\Components\Main\Dto\ViewDescriptor.h"

ViewDescriptor::ViewDescriptor(LayoutDescriptor layoutDescriptor, bool isResizeable)
{
    this->layoutDescriptor = layoutDescriptor;
    this->isResizeable = isResizeable;
}

ViewDescriptor::ViewDescriptor()
    : ViewDescriptor(LayoutDescriptor(), false)
{
}

LayoutDescriptor ViewDescriptor::GetLayoutDescriptor() const
{
    return layoutDescriptor;
}

void ViewDescriptor::SetLayoutDescriptor(LayoutDescriptor layoutDescriptor)
{
    this->layoutDescriptor = layoutDescriptor;
}

bool ViewDescriptor::IsResizeable() const
{
    return isResizeable;
}