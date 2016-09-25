#include "Windows\Framework\Dto\WindowDescriptor.h"

WindowDescriptor::WindowDescriptor()
{
    Position = Point(0, 0);
    WindowSize = Size(0, 0);

    OverflowX = OverflowModes::Fixed;
    OverflowY = OverflowModes::Fixed;

    AutoScale = false;
}

WindowDescriptor::WindowDescriptor(Point position, Size windowSize, OverflowModes overflowX, OverflowModes overflowY, bool autoScale)
{
    Position = position;
    WindowSize = windowSize;

    OverflowY = overflowY;
    OverflowX = overflowX;

    AutoScale = autoScale;
}

WindowDescriptor WindowDescriptor::CreateWindowDescriptor(Point position, Size windowSize, OverflowModes overflowX, OverflowModes overflowY, bool autoScale)
{
    return WindowDescriptor(position, windowSize, overflowX, overflowY, autoScale);
}

WindowDescriptor WindowDescriptor::CreateFixedWindowDescriptor(Point position, Size windowSize)
{
    return WindowDescriptor(position, windowSize, OverflowModes::Fixed, OverflowModes::Fixed);
}

WindowDescriptor WindowDescriptor::CreateStretchWindowDescriptor(Point position)
{
    return WindowDescriptor(position, Size(0, 0), OverflowModes::Stretch, OverflowModes::Stretch);
}