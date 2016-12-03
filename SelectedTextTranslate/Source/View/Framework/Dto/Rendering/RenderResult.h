#pragma once
#include "View\Framework\Dto\Rendering\RenderPosition.h"

class RenderResult
{
private:
    RenderPosition position;

public:
    RenderResult(RenderPosition position);
    RenderResult(Rect boundingRect);
    ~RenderResult();

    RenderPosition GetRenderPosition() const;
};
