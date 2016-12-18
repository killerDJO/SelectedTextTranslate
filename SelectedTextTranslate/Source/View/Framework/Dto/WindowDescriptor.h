#pragma once
#include "View\Framework\Enums\OverflowModes.h"
#include "View\Framework\Providers\ScaleProvider.h"
#include "View\Framework\Dto\Point\Point.h"
#include "View\Framework\Dto\Point\PointReal.h"
#include "View\Framework\Dto\Size\Size.h"
#include "View\Framework\Dto\Size\SizeReal.h"

class WindowDescriptor
{
protected:
    Point position;
    Size windowSize;

    OverflowModes overflowY;
    OverflowModes overflowX;

    wstring name;
    bool isEmpty;

public:
    WindowDescriptor();
    WindowDescriptor(Point position, Size windowSize, OverflowModes overflowX, OverflowModes overflowY, wstring name = wstring());
    WindowDescriptor(ScaleProvider* scaleProvider, PointReal position, SizeReal windowSize, OverflowModes overflowX, OverflowModes overflowY, wstring name = wstring());
    ~WindowDescriptor();

    static WindowDescriptor CreateWindowDescriptor(Point position, Size windowSize, OverflowModes overflowX, OverflowModes overflowY, wstring name = wstring());
    static WindowDescriptor CreateFixedWindowDescriptor(Point position, ::Size windowSize);
    static WindowDescriptor CreateStretchWindowDescriptor(Point position);

    static WindowDescriptor CreateWindowDescriptorDownscaled(ScaleProvider* scaleProvider, PointReal position, SizeReal windowSize, OverflowModes overflowX, OverflowModes overflowY, wstring name = wstring());
    static WindowDescriptor CreateFixedWindowDescriptorDownscaled(ScaleProvider* scaleProvider, PointReal position, SizeReal windowSize);
    static WindowDescriptor CreateStretchWindowDescriptorDownscaled(ScaleProvider* scaleProvider, PointReal position);

    Point GetPosition() const;
    Size GetWindowSize() const;
    OverflowModes GetOverflowY() const;
    OverflowModes GetOverflowX() const;
    wstring GetName() const;

    void SetPosition(Point position);
    void SetWindowSize(Size windowSize);

    bool IsEmpty() const;
};
