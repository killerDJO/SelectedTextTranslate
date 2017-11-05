#pragma once
#include "Presentation\Framework\Dto\Positioning\Point.h"
#include "Presentation\Framework\Dto\Positioning\Size.h"
#include "Presentation\Framework\Enums\OverflowModes.h"

class LayoutDescriptor
{
private:
    Point position;
    Size size;

    OverflowModes overflowY;
    OverflowModes overflowX;

    bool isEmpty;

public:
    LayoutDescriptor();
    LayoutDescriptor(Point position, Size size, OverflowModes overflowX, OverflowModes overflowY);

    static LayoutDescriptor CreateLayoutDescriptor(Point position, Size size, OverflowModes overflowX, OverflowModes overflowY);
    static LayoutDescriptor CreateFixedLayoutDescriptor(Point position, Size size);
    static LayoutDescriptor CreateStretchViewLayout(Point position);

    Point GetPosition() const;
    Size GetSize() const;
    OverflowModes GetOverflowY() const;
    OverflowModes GetOverflowX() const;

    void SetPosition(Point position);
    void SetSize(Size size);

    bool IsEmpty() const;
    bool IsFixedLayout() const;
};
