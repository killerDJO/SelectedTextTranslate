#pragma once

class PointReal
{
private:
    double x;
    double y;

public:
    PointReal();
    PointReal(double x, double y);

    double GetX() const;
    double GetY() const;
};