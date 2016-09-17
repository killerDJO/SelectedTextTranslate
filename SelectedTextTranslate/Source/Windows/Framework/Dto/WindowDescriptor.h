#pragma once
#include "Windows\Framework\Enums\OverflowModes.h"

struct WindowDescriptor
{
    int X;
    int Y;

    int Width;
    int Height;

    OverflowModes OverflowY;
    OverflowModes OverflowX;

    static WindowDescriptor CreateWindowDescriptor(int x, int y, int width, int height, OverflowModes overflowX, OverflowModes overflowY);
    static WindowDescriptor CreateFixedWindowDescriptor(int x, int y, int width, int height);
    static WindowDescriptor CreateStretchWindowDescriptor(int x, int y);

    WindowDescriptor();
    WindowDescriptor(int x, int y, int width, int height, OverflowModes overflowX, OverflowModes overflowY);
};