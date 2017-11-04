#include "Presentation\Framework\Dto\Positioning\Size.h"

Size::Size()
{
    this->width = this->height = 0;
}

Size::Size(int width, int height)
{
    this->width = width;
    this->height = height;
}

int Size::GetWidth() const
{
    return width;
}

int Size::GetHeight() const
{
    return height;
}

bool Size::Equals(Size size) const
{
    return size.width == width && size.height == height;
}

Size Size::Max(Size otherSize) const
{
    return Size(
        max(width, otherSize.GetWidth()),
        max(height, otherSize.GetHeight())
    );
}