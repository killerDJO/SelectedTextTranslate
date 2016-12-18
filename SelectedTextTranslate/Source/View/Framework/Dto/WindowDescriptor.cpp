#include "View\Framework\Dto\WindowDescriptor.h"

WindowDescriptor::WindowDescriptor()
{
    this->position = Point(0, 0);
    this->windowSize = Size(0, 0);

    this->overflowX = OverflowModes::Fixed;
    this->overflowY = OverflowModes::Fixed;

    this->isEmpty = true;
}

WindowDescriptor::WindowDescriptor(Point position, Size windowSize, OverflowModes overflowX, OverflowModes overflowY, wstring name)
{
    this->position = position;
    this->windowSize = windowSize;

    this->overflowY = overflowY;
    this->overflowX = overflowX;

    this->name = name;
    this->isEmpty = false;
}

WindowDescriptor::WindowDescriptor(ScaleProvider* scaleProvider, PointReal position, SizeReal windowSize, OverflowModes overflowX, OverflowModes overflowY, wstring name)
{
    this->position = scaleProvider->Scale(position);
    this->windowSize = scaleProvider->Scale(windowSize);

    this->overflowY = overflowY;
    this->overflowX = overflowX;

    this->name = name;
    this->isEmpty = false;
}

WindowDescriptor WindowDescriptor::CreateWindowDescriptor(Point position, Size windowSize, OverflowModes overflowX, OverflowModes overflowY, wstring name)
{
    return WindowDescriptor(position, windowSize, overflowX, overflowY, name);
}

WindowDescriptor WindowDescriptor::CreateFixedWindowDescriptor(Point position, Size windowSize)
{
    return WindowDescriptor(position, windowSize, OverflowModes::Fixed, OverflowModes::Fixed);
}

WindowDescriptor WindowDescriptor::CreateStretchWindowDescriptor(Point position)
{
    return WindowDescriptor(position, Size(0, 0), OverflowModes::Stretch, OverflowModes::Stretch);
}

WindowDescriptor WindowDescriptor::CreateWindowDescriptorDownscaled(ScaleProvider* scaleProvider, PointReal position, SizeReal windowSize, OverflowModes overflowX, OverflowModes overflowY, wstring name)
{
    return WindowDescriptor(scaleProvider, position, windowSize, overflowX, overflowY, name);
}

WindowDescriptor WindowDescriptor::CreateFixedWindowDescriptorDownscaled(ScaleProvider* scaleProvider, PointReal position, SizeReal windowSize)
{
    return WindowDescriptor(scaleProvider, position, windowSize, OverflowModes::Fixed, OverflowModes::Fixed);
}

WindowDescriptor WindowDescriptor::CreateStretchWindowDescriptorDownscaled(ScaleProvider* scaleProvider, PointReal position)
{
    return WindowDescriptor(scaleProvider, position, SizeReal(0, 0), OverflowModes::Stretch, OverflowModes::Stretch);
}

Point WindowDescriptor::GetPosition() const
{
    return position;
}

Size WindowDescriptor::GetWindowSize() const
{
    return windowSize;
}

OverflowModes WindowDescriptor::GetOverflowY() const
{
    return overflowY;
}

OverflowModes WindowDescriptor::GetOverflowX() const
{
    return overflowX;
}

wstring WindowDescriptor::GetName() const
{
    return name;
}

void WindowDescriptor::SetPosition(Point position)
{
    this->position = position;
}

void WindowDescriptor::SetWindowSize(Size windowSize)
{
    this->windowSize = windowSize;
}

bool WindowDescriptor::IsEmpty() const
{
    return isEmpty;
}

WindowDescriptor::~WindowDescriptor()
{
}