#pragma once

class SizeReal
{
private:
    double width;
    double height;

public:
    SizeReal();
    SizeReal(double width, double height);

    double GetWidth() const;
    double GetHeight() const;
};