#pragma once

class Point
{
private:
    int x;
    int y;

public:
    Point();
    Point(int x, int y);

    int GetX() const;
    int GetY() const;
};