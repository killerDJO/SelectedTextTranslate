#pragma once
#include "View\Framework\Rendering\Dto\RenderPosition.h"
#include "View\Framework\Dto\Rect\RectReal.h"

class RenderResult
{
private:
    RenderPosition position;

public:
    RenderResult(RenderPosition position);
    RenderResult(RectReal boundingRect);

    RenderPosition GetRenderPosition() const;
};
