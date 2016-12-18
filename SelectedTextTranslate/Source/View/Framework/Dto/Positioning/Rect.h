#pragma once
#include "View\Framework\Dto\Positioning\Point.h"
#include "View\Framework\Dto\Positioning\Size.h"

class Rect
{
private:
    Point position;
    Size size;

public:
    Rect();
    Rect(int x, int y, int width, int height);
    Rect(Point position, Size size);

    Point GetPosition() const;
    Size GetSize() const;

    int GetX() const;
    int GetY() const;
    int GetWidth() const;
    int GetHeight() const;
    int GetRight() const;
    int GetBottom() const;
};