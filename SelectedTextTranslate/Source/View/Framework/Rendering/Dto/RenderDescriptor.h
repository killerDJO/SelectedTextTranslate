#pragma once
#include "View\Framework\Rendering\Dto\RenderPosition.h"
#include "View\Framework\Rendering\Renderer.h"

class RenderDescriptor
{
private:
    Renderer* renderer;
    RenderPosition position;

public:
    RenderDescriptor(Renderer* renderer, RenderPosition startPosition);
    RenderDescriptor(Renderer* renderer, TextRenderResult textRenderResult);
    ~RenderDescriptor();

    Renderer* GetRenderer() const;
    RenderPosition GetRenderPosition() const;
};
