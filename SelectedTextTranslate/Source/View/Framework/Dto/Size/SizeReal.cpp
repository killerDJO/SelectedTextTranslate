#include "View\Framework\Dto\Size\SizeReal.h"

SizeReal::SizeReal()
{
    this->width = this->height = 0;
}

SizeReal::SizeReal(double width, double height)
{
    this->width = width;
    this->height = height;
}

double SizeReal::GetWidth() const
{
    return width;
}

double SizeReal::GetHeight() const
{
    return height;
}