#include "Presentation\Components\Main\Dto\ViewDescriptor.h"

ViewDescriptor::ViewDescriptor(LayoutDescriptor windowDescriptor, bool isResizeable)
{
    this->windowDescriptor = windowDescriptor;
    this->isResizeable = isResizeable;
}

ViewDescriptor::ViewDescriptor()
    : ViewDescriptor(LayoutDescriptor(), false)
{
}

LayoutDescriptor ViewDescriptor::GetWindowDescriptor() const
{
    return windowDescriptor;
}

void ViewDescriptor::SetWindowDescriptor(LayoutDescriptor windowDescriptor)
{
    this->windowDescriptor = windowDescriptor;
}

bool ViewDescriptor::IsResizeable() const
{
    return isResizeable;
}