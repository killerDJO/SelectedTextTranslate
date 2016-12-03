#include "View\Framework\Rendering\Dto\RenderPosition.h"
#include "View\Framework\Rendering\Dto\TextRenderResult.h"
#include "View\Framework\Rendering\Dto\RenderResult.h"

RenderPosition::RenderPosition(Point initialPosition)
{
    position = initialPosition;
}

RenderPosition::RenderPosition(TextRenderResult textRenderResult)
{
    position = textRenderResult.GetRight();
}

RenderPosition::RenderPosition(RenderResult renderResult)
{
    position = renderResult.GetRenderPosition().GetPosition();
}

RenderPosition::RenderPosition(int x, int y)
    : RenderPosition(Point(x, y))
{
}

RenderPosition::RenderPosition()
    : RenderPosition(Point(0, 0))
{
}

Point RenderPosition::GetPosition() const
{
    return position;
}

int RenderPosition::GetY() const
{
    return position.Y;
}

int RenderPosition::GetX() const
{
    return position.X;
}

RenderPosition RenderPosition::SetX(int x) const
{
    return RenderPosition(Point(x, position.Y));
}

RenderPosition RenderPosition::SetY(int y) const
{
    return RenderPosition(Point(position.X, y));
}

RenderPosition RenderPosition::MoveX(int x) const
{
    return RenderPosition(Point(position.X + x, position.Y));
}

RenderPosition RenderPosition::MoveY(int y) const
{
    return RenderPosition(Point(position.X, position.Y + y));
}

RenderPosition::~RenderPosition()
{
}
