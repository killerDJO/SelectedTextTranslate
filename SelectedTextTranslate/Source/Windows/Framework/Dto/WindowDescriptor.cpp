#include "Windows\Framework\Dto\WindowDescriptor.h"

WindowDescriptor::WindowDescriptor()
{
    X = 0;
    Y = 0;
    Width = 0;
    Height = 0;
    OverflowX = OverflowModes::Fixed;
    OverflowY = OverflowModes::Fixed;
}

WindowDescriptor::WindowDescriptor(int x, int y, int width, int height, OverflowModes overflowX, OverflowModes overflowY)
{
    X = x;
    Y = y;

    Width = width;
    Height = height;

    OverflowY = overflowY;
    OverflowX = overflowX;
}

WindowDescriptor WindowDescriptor::CreateWindowDescriptor(int x, int y, int width, int height, OverflowModes overflowX, OverflowModes overflowY)
{
    return WindowDescriptor(x, y, width, height, overflowX, overflowY);
}

WindowDescriptor WindowDescriptor::CreateFixedWindowDescriptor(int x, int y, int width, int height)
{
    return WindowDescriptor(x, y, width, height, OverflowModes::Fixed, OverflowModes::Fixed);
}

WindowDescriptor WindowDescriptor::CreateStretchWindowDescriptor(int x, int y)
{
    return WindowDescriptor(x, y, 0, 0, OverflowModes::Stretch, OverflowModes::Stretch);
}