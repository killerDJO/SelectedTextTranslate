#include "View\Framework\Rendering\RenderingContext.h"

RenderingContext::RenderingContext(RenderingProvider* renderingProvider, DeviceContextProvider* deviceContextProvider)
{
    this->deviceContextProvider = deviceContextProvider;
    this->renderingProvider = renderingProvider;

    this->renderingRoot = nullptr;
}

Renderer* RenderingContext::GetRenderer() const
{
    return new Renderer(renderingProvider, deviceContextProvider);
}

void RenderingContext::ReleaseRenderer(Renderer* renderer) const
{
    delete renderer;
}

void RenderingContext::BeginRender(View* window)
{
    if(renderingRoot == nullptr)
    {
        renderingRoot = window;
    }
}

void RenderingContext::EndRender(View* window)
{
    if(window == renderingRoot)
    {
        renderingRoot = nullptr;
    }
}

bool RenderingContext::IsRenderingRoot(View* window) const
{
    return window == renderingRoot;
}