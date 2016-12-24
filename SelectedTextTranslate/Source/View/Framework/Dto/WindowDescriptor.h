#pragma once
#include "View\Framework\Dto\Positioning\Point.h"
#include "View\Framework\Dto\Positioning\Size.h"
#include "View\Framework\Enums\OverflowModes.h"

class WindowDescriptor
{
private:
    Point position;
    Size windowSize;

    OverflowModes overflowY;
    OverflowModes overflowX;

    wstring name;
    bool isEmpty;

public:
    WindowDescriptor();
    WindowDescriptor(Point position, Size windowSize, OverflowModes overflowX, OverflowModes overflowY, wstring name = wstring());

    static WindowDescriptor CreateWindowDescriptor(Point position, Size windowSize, OverflowModes overflowX, OverflowModes overflowY, wstring name = wstring());
    static WindowDescriptor CreateFixedWindowDescriptor(Point position, Size windowSize);
    static WindowDescriptor CreateStretchWindowDescriptor(Point position);

    Point GetPosition() const;
    Size GetSize() const;
    OverflowModes GetOverflowY() const;
    OverflowModes GetOverflowX() const;
    wstring GetName() const;

    void SetPosition(Point position);
    void SetWindowSize(Size windowSize);

    bool IsEmpty() const;
};
