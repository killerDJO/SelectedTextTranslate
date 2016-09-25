#pragma once
#include "Windows\Framework\Enums\OverflowModes.h"

struct WindowDescriptor
{
    Point Position;
    Size WindowSize;

    OverflowModes OverflowY;
    OverflowModes OverflowX;

    bool AutoScale;

    static WindowDescriptor CreateWindowDescriptor(Point position, Size windowSize, OverflowModes overflowX, OverflowModes overflowY, bool autoScale = true);
    static WindowDescriptor CreateFixedWindowDescriptor(Point position, Size windowSize);
    static WindowDescriptor CreateStretchWindowDescriptor(Point position);

    WindowDescriptor();
    WindowDescriptor(Point position, Size windowSize, OverflowModes overflowX, OverflowModes overflowY, bool autoScale = true);
};