#include "View\Framework\Rendering\Dto\RenderPosition.h"
#include "View\Framework\Rendering\Dto\TextRenderResult.h"
#include "View\Framework\Rendering\Dto\RenderResult.h"
#include "View\Framework\Dto\Point\PointReal.h"

RenderPosition::RenderPosition(PointReal initialPosition)
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

RenderPosition::RenderPosition(double x, double y)
    : RenderPosition(PointReal(x, y))
{
}

RenderPosition::RenderPosition()
    : RenderPosition(PointReal(0, 0))
{
}

PointReal RenderPosition::GetPosition() const
{
    return position;
}

double RenderPosition::GetY() const
{
    return position.GetY();
}

double RenderPosition::GetX() const
{
    return position.GetX();
}

RenderPosition RenderPosition::SetX(double x) const
{
    return RenderPosition(PointReal(x, position.GetY()));
}

RenderPosition RenderPosition::SetY(double y) const
{
    return RenderPosition(PointReal(position.GetX(), y));
}

RenderPosition RenderPosition::MoveX(double x) const
{
    return RenderPosition(PointReal(position.GetX() + x, position.GetY()));
}

RenderPosition RenderPosition::MoveY(double y) const
{
    return RenderPosition(PointReal(position.GetX(), position.GetY() + y));
}
