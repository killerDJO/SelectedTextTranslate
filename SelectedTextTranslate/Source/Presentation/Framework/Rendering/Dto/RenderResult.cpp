#include "Presentation\Framework\Rendering\Dto\RenderResult.h"

RenderResult::RenderResult(RenderPosition position)
{
    this->position = position;
}

RenderResult::RenderResult(Rect boundingRect)
    : RenderResult(Point(boundingRect.GetRight(), boundingRect.GetBottom()))
{
}

RenderPosition RenderResult::GetRenderPosition() const
{
    return position;
}
