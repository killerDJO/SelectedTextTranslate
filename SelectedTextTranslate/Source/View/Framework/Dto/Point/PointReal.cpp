#include "View\Framework\Dto\Point\PointReal.h"

PointReal::PointReal()
{
    this->x = this->y = 0;
}

PointReal::PointReal(double x, double y)
{
    this->x = x;
    this->y = y;
}

double PointReal::GetX() const
{
    return x;
}

double PointReal::GetY() const
{
    return y;
}