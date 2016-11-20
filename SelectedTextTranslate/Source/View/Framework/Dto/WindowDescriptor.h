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
    wstring name;

    bool isEmpty;

public:
    WindowDescriptor();
    WindowDescriptor(Point position, Size windowSize, OverflowModes overflowX, OverflowModes overflowY, bool autoScale = true, wstring name = wstring());
    ~WindowDescriptor();

    static WindowDescriptor CreateWindowDescriptor(Point position, Size windowSize, OverflowModes overflowX, OverflowModes overflowY, bool autoScale = true, wstring name = wstring());
    static WindowDescriptor CreateFixedWindowDescriptor(Point position, Size windowSize);
    static WindowDescriptor CreateStretchWindowDescriptor(Point position);

    Point GetPosition() const;
    Size GetWindowSize() const;
    OverflowModes GetOverflowY() const;
    OverflowModes GetOverflowX() const;
    bool IsAutoScaleEnabled() const;
    wstring GetName() const;

    void SetPosition(Point position);
    void SetWindowSize(Size windowSize);

    bool IsEmpty() const;
};