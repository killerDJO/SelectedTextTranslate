#include "View\Framework\Rendering\Dto\RenderResult.h"

RenderResult::RenderResult(RenderPosition position)
{
    this->position = position;
}

RenderResult::RenderResult(RectReal boundingRect)
    : RenderResult(PointReal(boundingRect.GetRight(), boundingRect.GetBottom()))
{
}

RenderPosition RenderResult::GetRenderPosition() const
{
    return position;
}
