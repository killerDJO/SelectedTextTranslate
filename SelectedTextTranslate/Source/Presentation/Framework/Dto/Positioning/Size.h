#pragma once

class Size
{
private:
    int width;
    int height;

public:
    Size();
    Size(int width, int height);

    int GetWidth() const;
    int GetHeight() const;

    bool Equals(Size size) const;
    Size Max(Size otherSize) const;
};