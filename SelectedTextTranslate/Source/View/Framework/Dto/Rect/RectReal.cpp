#include "View\Framework\Dto\Rect\RectReal.h"

RectReal::RectReal()
{
    this->position = PointReal();
    this->size = SizeReal();
}

RectReal::RectReal(double x, double y, double width, double height)
{
    this->position = PointReal(x, y);
    this->size = SizeReal(width, height);
}

RectReal::RectReal(PointReal position, SizeReal size)
{
    this->position = position;
    this->size = size;
}

PointReal RectReal::GetPosition() const
{
    return position;
}

SizeReal RectReal::GetSize() const
{
    return size;
}

double RectReal::GetX() const
{
    return position.GetX();
}

double RectReal::GetY() const
{
    return position.GetY();
}

double RectReal::GetWidth() const
{
    return size.GetWidth();
}

double RectReal::GetHeight() const
{
    return size.GetHeight();
}

double RectReal::GetRight() const
{
    return position.GetX() + size.GetWidth();
}

double RectReal::GetBottom() const
{
    return position.GetY() + size.GetHeight();
}
