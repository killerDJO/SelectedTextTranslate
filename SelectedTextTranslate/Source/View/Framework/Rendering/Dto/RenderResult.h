#pragma once
#include "View\Framework\Rendering\Dto\RenderPosition.h"
#include "View\Framework\Dto\Positioning\Rect.h"

class RenderResult
{
private:
    RenderPosition position;

public:
    RenderResult(RenderPosition position);
    RenderResult(Rect boundingRect);

    RenderPosition GetRenderPosition() const;
};
