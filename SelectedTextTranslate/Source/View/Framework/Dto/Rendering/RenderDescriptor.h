#pragma once
#include "View\Framework\Renderer.h"
#include "View\Framework\Dto\Rendering\TextRenderResult.h"
#include "View\Framework\Dto\Rendering\RenderPosition.h"

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
