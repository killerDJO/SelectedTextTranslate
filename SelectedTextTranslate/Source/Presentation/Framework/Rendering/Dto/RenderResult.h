#pragma once
#include "Presentation\Framework\Dto\Positioning\Rect.h"
#include "Presentation\Framework\Rendering\Dto\RenderPosition.h"

class RenderResult
{
private:
    RenderPosition position;

public:
    RenderResult(RenderPosition position);
    RenderResult(Rect boundingRect);

    RenderPosition GetRenderPosition() const;
};
