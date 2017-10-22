#pragma once

class Pen
{
private:
    HPEN handle;

public:
    Pen(HPEN handle);
    ~Pen();

    HPEN GetHandle() const;
};