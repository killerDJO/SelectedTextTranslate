#pragma once
#include "View\Framework\Dto\Point\PointReal.h"
#include "View\Framework\Dto\Size\SizeReal.h"

class RectReal
{
private:
    PointReal position;
    SizeReal size;

public:
    RectReal();
    RectReal(double x, double y, double width, double height);
    RectReal(PointReal position, SizeReal size);

    PointReal GetPosition() const;
    SizeReal GetSize() const;

    double GetX() const;
    double GetY() const;
    double GetWidth() const;
    double GetHeight() const;
    double GetRight() const;
    double GetBottom() const;
};