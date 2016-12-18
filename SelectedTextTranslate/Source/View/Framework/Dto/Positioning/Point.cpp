#include "View\Framework\Dto\Positioning\\Point.h"

Point::Point()
{
    this->x = this->y = 0;
}

Point::Point(int x, int y)
{
    this->x = x;
    this->y = y;
}

int Point::GetX() const
{
    return x;
}

int Point::GetY() const
{
    return y;
}
