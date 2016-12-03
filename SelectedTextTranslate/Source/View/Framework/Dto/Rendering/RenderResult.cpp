#include "View\Framework\Dto\Rendering\RenderResult.h"

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

RenderResult::~RenderResult()
{
}
