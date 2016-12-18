#pragma once

class Brush
{
private:
    HBRUSH handle;

public:
    Brush(HBRUSH handle);
    ~Brush();

    HBRUSH GetHandle() const;
};