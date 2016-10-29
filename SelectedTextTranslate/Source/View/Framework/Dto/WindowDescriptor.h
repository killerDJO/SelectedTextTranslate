#pragma once
#include "View\Framework\Enums\OverflowModes.h"

class WindowDescriptor
{
private:
    Point position;

    Size windowSize;

    OverflowModes overflowY;
    OverflowModes overflowX;

    bool autoScale;

public:
    WindowDescriptor();
    WindowDescriptor(Point position, Size windowSize, OverflowModes overflowX, OverflowModes overflowY, bool autoScale = true);
    ~WindowDescriptor();

    static WindowDescriptor CreateWindowDescriptor(Point position, Size windowSize, OverflowModes overflowX, OverflowModes overflowY, bool autoScale = true);
    static WindowDescriptor CreateFixedWindowDescriptor(Point position, Size windowSize);
    static WindowDescriptor CreateStretchWindowDescriptor(Point position);

    Point GetPosition() const;
    Size GetWindowSize() const;
    OverflowModes GetOverflowY() const;
    OverflowModes GetOverflowX() const;
    bool IsAutoScaleEnabled() const;

    void SetPosition(Point position);
    void SetWindowSize(Size windowSize);
};