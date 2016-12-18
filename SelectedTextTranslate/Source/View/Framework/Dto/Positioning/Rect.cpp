#include "View\Framework\Dto\Positioning\Rect.h"

Rect::Rect()
{
    this->position = Point();
    this->size = Size();
}

Rect::Rect(int x, int y, int width, int height)
{
    this->position = Point(x, y);
    this->size = Size(width, height);
}

Rect::Rect(Point position, Size size)
{
    this->position = position;
    this->size = size;
}

Point Rect::GetPosition() const
{
    return position;
}

Size Rect::GetSize() const
{
    return size;
}

int Rect::GetX() const
{
    return position.GetX();
}

int Rect::GetY() const
{
    return position.GetY();
}

int Rect::GetWidth() const
{
    return size.GetWidth();
}

int Rect::GetHeight() const
{
    return size.GetHeight();
}

int Rect::GetRight() const
{
    return position.GetX() + size.GetWidth();
}

int Rect::GetBottom() const
{
    return position.GetY() + size.GetHeight();
}
