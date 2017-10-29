#pragma once
#include "Presentation\Framework\Dto\Positioning\Point.h"
#include "Presentation\Framework\Dto\Positioning\Size.h"
#include "Presentation\Framework\Dto\Positioning\Rect.h"
#include "Presentation\Framework\Enums\ViewStates.h"

class ViewStateDescriptor
{
private:
    Size size;
    Size contentSize;
    Point position;
    bool isVisible;
    ViewStates viewState;

public:
    ViewStateDescriptor();
    ViewStateDescriptor(Size size, Point position, bool isVisible);

    Size GetSize() const;
    void SetSize(Size size);

    Size GetContentSize() const;
    void SetContentSize(Size contentSize);

    Point GetPosition() const;
    void SetPosition(Point position);

    ViewStates GetViewState() const;
    void SetViewState(ViewStates viewState);

    Rect GetBoundingRect() const;

    void MakeVisible();
    void MakeHidden();
    bool IsVisible() const;

    void SetWidth(int width);
    void SetHeight(int height);
    void EnsureSize(Size size);
};
