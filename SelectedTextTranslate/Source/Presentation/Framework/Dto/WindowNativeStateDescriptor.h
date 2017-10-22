#pragma once
#include "Presentation\Framework\Dto\Positioning\Point.h"
#include "Presentation\Framework\Dto\Positioning\Size.h"
#include "Presentation\Framework\Dto\Positioning\Rect.h"

class WindowNativeStateDescriptor
{
private:
    Size size;
    Point position;
    bool isVisible;

public:
    WindowNativeStateDescriptor();
    WindowNativeStateDescriptor(Size size, Point position, bool isVisible);

    Size GetSize() const;
    void SetSize(Size size);

    Point GetPosition() const;
    void SetPosition(Point position);

    Rect GetBoundingRect() const;

    void MakeVisible();
    void MakeHidden();
    bool IsVisible() const;

    void EnsureWidth(int width);
    void EnsureHeight(int height);
    void EnsureSize(Size size);
};