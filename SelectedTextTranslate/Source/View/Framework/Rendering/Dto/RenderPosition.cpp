#include "View\Framework\Rendering\Dto\RenderPosition.h"
#include "View\Framework\Rendering\Dto\TextRenderResult.h"
#include "View\Framework\Rendering\Dto\RenderResult.h"
#include "View\Framework\Providers\ScaleProvider.h"

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
    return position.GetY();
}

int RenderPosition::GetX() const
{
    return position.GetX();
}

RenderPosition RenderPosition::SetX(int x) const
{
    return RenderPosition(Point(x, position.GetY()));
}

RenderPosition RenderPosition::SetX(int x, ScaleProvider* scaleProvider) const
{
    return SetX(scaleProvider->Scale(x));
}

RenderPosition RenderPosition::SetY(int y) const
{
    return RenderPosition(Point(position.GetX(), y));
}

RenderPosition RenderPosition::SetY(int y, ScaleProvider* scaleProvider) const
{
    return SetY(scaleProvider->Scale(y));
}

RenderPosition RenderPosition::MoveX(int x) const
{
    return RenderPosition(Point(position.GetX() + x, position.GetY()));
}

RenderPosition RenderPosition::MoveX(int x, ScaleProvider* scaleProvider) const
{
    return MoveX(scaleProvider->Scale(x));
}

RenderPosition RenderPosition::MoveY(int y) const
{
    return RenderPosition(Point(position.GetX(), position.GetY() + y));
}

RenderPosition RenderPosition::MoveY(int y, ScaleProvider* scaleProvider) const
{
    return MoveY(scaleProvider->Scale(y));
}
