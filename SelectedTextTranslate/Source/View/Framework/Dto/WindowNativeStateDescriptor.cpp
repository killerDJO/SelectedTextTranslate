#include "View\Framework\Dto\WindowNativeStateDescriptor.h"

WindowNativeStateDescriptor::WindowNativeStateDescriptor()
{
    size = Size(0, 0);
    position = Point(0, 0);
    isVisible = false;
}

WindowNativeStateDescriptor::WindowNativeStateDescriptor(Size size, Point position, bool isVisible)
{
    this->size = size;
    this->position = position;
    this->isVisible = isVisible;
}

Size WindowNativeStateDescriptor::GetSize() const
{
    return size;
}

void WindowNativeStateDescriptor::SetSize(Size size)
{
    this->size = size;
}

Point WindowNativeStateDescriptor::GetPosition() const
{
    return position;
}

void WindowNativeStateDescriptor::SetPosition(Point position)
{
    this->position = position;
}

Rect WindowNativeStateDescriptor::GetBoundingRect() const
{
    return Rect(position, size);
}

void WindowNativeStateDescriptor::MakeVisible()
{
    isVisible = true;
}

void WindowNativeStateDescriptor::MakeHidden()
{
    isVisible = false;
}

bool WindowNativeStateDescriptor::IsVisible() const
{
    return isVisible;
}

void WindowNativeStateDescriptor::EnsureWidth(int width)
{
    if(size.GetWidth() < width)
    {
        size = Size(width, size.GetHeight());
    }
}

void WindowNativeStateDescriptor::EnsureHeight(int height)
{
    if (size.GetHeight() < height)
    {
        size = Size(size.GetWidth(), height);
    }
}

void WindowNativeStateDescriptor::EnsureSize(Size size)
{
    EnsureWidth(size.GetWidth());
    EnsureHeight(size.GetHeight());
}
