#include "Presentation\Framework\Dto\ViewStateDescriptor.h"

ViewStateDescriptor::ViewStateDescriptor()
{
    size = Size(0, 0);
    position = Point(0, 0);
    viewState = ViewStates::New;
    isVisible = false;
}

ViewStateDescriptor::ViewStateDescriptor(Size size, Point position, bool isVisible)
{
    this->size = size;
    this->position = position;
    this->isVisible = isVisible;
    viewState = ViewStates::New;
}

Size ViewStateDescriptor::GetSize() const
{
    return size;
}

void ViewStateDescriptor::SetSize(Size size)
{
    this->size = size;
}

Size ViewStateDescriptor::GetContentSize() const
{
    return contentSize;
}

void ViewStateDescriptor::SetContentSize(Size contentSize)
{
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
    return Rect(position, size);
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

void ViewStateDescriptor::SetWidth(int width)
{
    size = Size(width, size.GetHeight());
}

void ViewStateDescriptor::SetHeight(int height)
{
    size = Size(size.GetWidth(), height);
}

void ViewStateDescriptor::EnsureSize(Size size)
{
    SetWidth(size.GetWidth());
    SetHeight(size.GetHeight());
}
