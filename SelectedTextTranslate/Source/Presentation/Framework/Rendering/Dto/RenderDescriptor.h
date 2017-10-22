#pragma once
#include "Presentation\Framework\Rendering\Dto\RenderPosition.h"
#include "Presentation\Framework\Rendering\Renderer.h"

class RenderDescriptor
{
private:
    Renderer* renderer;
    RenderPosition position;

public:
    RenderDescriptor(Renderer* renderer, RenderPosition startPosition);
    RenderDescriptor(Renderer* renderer, TextRenderResult textRenderResult);

    Renderer* GetRenderer() const;
    RenderPosition GetRenderPosition() const;
};
