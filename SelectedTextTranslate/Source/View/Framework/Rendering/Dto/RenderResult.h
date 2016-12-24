#pragma once
#include "View\Framework\Dto\Positioning\Rect.h"
#include "View\Framework\Rendering\Dto\RenderPosition.h"

class RenderResult
{
private:
    RenderPosition position;

public:
    RenderResult(RenderPosition position);
    RenderResult(Rect boundingRect);

    RenderPosition GetRenderPosition() const;
};
