#include "Presentation\Framework\Dto\LayoutDescriptor.h"

LayoutDescriptor::LayoutDescriptor()
{
    this->position = Point(0, 0);
    this->size = Size(0, 0);

    this->overflowX = OverflowModes::Fixed;
    this->overflowY = OverflowModes::Fixed;

    this->isEmpty = true;
}

LayoutDescriptor::LayoutDescriptor(Point position, Size size, OverflowModes overflowX, OverflowModes overflowY)
{
    this->position = position;
    this->size = size;

    this->overflowY = overflowY;
    this->overflowX = overflowX;

    this->isEmpty = false;
}

LayoutDescriptor LayoutDescriptor::CreateLayoutDescriptor(Point position, Size windowSize, OverflowModes overflowX, OverflowModes overflowY)
{
    return LayoutDescriptor(position, windowSize, overflowX, overflowY);
}

LayoutDescriptor LayoutDescriptor::CreateFixedLayoutDescriptor(Point position, Size windowSize)
{
    return LayoutDescriptor(position, windowSize, OverflowModes::Fixed, OverflowModes::Fixed);
}

LayoutDescriptor LayoutDescriptor::CreateStretchViewLayout(Point position)
{
    return LayoutDescriptor(position, Size(0, 0), OverflowModes::Stretch, OverflowModes::Stretch);
}

Point LayoutDescriptor::GetPosition() const
{
    return position;
}

Size LayoutDescriptor::GetSize() const
{
    return size;
}

OverflowModes LayoutDescriptor::GetOverflowY() const
{
    return overflowY;
}

OverflowModes LayoutDescriptor::GetOverflowX() const
{
    return overflowX;
}

void LayoutDescriptor::SetPosition(Point position)
{
    this->position = position;
}

void LayoutDescriptor::SetSize(Size windowSize)
{
    this->size = windowSize;
}

bool LayoutDescriptor::IsEmpty() const
{
    return isEmpty;
}