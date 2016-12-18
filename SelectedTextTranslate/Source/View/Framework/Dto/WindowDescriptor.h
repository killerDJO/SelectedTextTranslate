#pragma once
#include "View\Framework\Enums\OverflowModes.h"
#include "View\Framework\Dto\Positioning\Point.h"
#include "View\Framework\Dto\Positioning\Size.h"

class WindowDescriptor
{
protected:
    Point position;
    Size windowSize;

    OverflowModes overflowY;
    OverflowModes overflowX;

    wstring name;
    bool isEmpty;

public:
    WindowDescriptor();
    WindowDescriptor(Point position, Size windowSize, OverflowModes overflowX, OverflowModes overflowY, wstring name = wstring());
    ~WindowDescriptor();

    static WindowDescriptor CreateWindowDescriptor(Point position, Size windowSize, OverflowModes overflowX, OverflowModes overflowY, wstring name = wstring());
    static WindowDescriptor CreateFixedWindowDescriptor(Point position, ::Size windowSize);
    static WindowDescriptor CreateStretchWindowDescriptor(Point position);

    Point GetPosition() const;
    Size GetWindowSize() const;
    OverflowModes GetOverflowY() const;
    OverflowModes GetOverflowX() const;
    wstring GetName() const;

    void SetPosition(Point position);
    void SetWindowSize(Size windowSize);

    bool IsEmpty() const;
};
