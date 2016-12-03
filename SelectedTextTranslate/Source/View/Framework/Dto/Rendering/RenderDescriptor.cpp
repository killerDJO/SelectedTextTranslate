#include "View\Framework\Dto\Rendering\RenderDescriptor.h"

RenderDescriptor::RenderDescriptor(Renderer* renderer, RenderPosition startPosition)
{
    this->renderer = renderer;
    this->position = startPosition;
}

RenderDescriptor::RenderDescriptor(Renderer* renderer, TextRenderResult textRenderResult)
    : RenderDescriptor(renderer, textRenderResult.GetRenderPosition())
{
}

Renderer* RenderDescriptor::GetRenderer() const
{
    return renderer;
}

RenderPosition RenderDescriptor::GetRenderPosition() const
{
    return position;
}

RenderDescriptor::~RenderDescriptor()
{
}