#include "View\Content\Main\Dto\ViewDescriptor.h"

ViewDescriptor::ViewDescriptor(WindowDescriptor windowDescriptor, bool isResizeable)
{
    this->windowDescriptor = windowDescriptor;
    this->isResizeable = isResizeable;
}

ViewDescriptor::ViewDescriptor()
    : ViewDescriptor(WindowDescriptor(), false)
{
}

WindowDescriptor ViewDescriptor::GetWindowDescriptor() const
{
    return windowDescriptor;
}

void ViewDescriptor::SetWindowDescriptor(WindowDescriptor windowDescriptor)
{
    this->windowDescriptor = windowDescriptor;
}

bool ViewDescriptor::IsResizeable() const
{
    return isResizeable;
}