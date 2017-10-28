#include "Presentation\Framework\Rendering\RenderingContext.h"

RenderingContext::RenderingContext(ServiceRegistry* registry)
{
    this->deviceContextProvider = registry->Get<DeviceContextProvider>();
    this->renderingProvider = registry->Get<RenderingProvider>();

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

bool RenderingContext::IsRenderingInProgress() const
{
    return renderingRoot != nullptr;
}

bool RenderingContext::IsRenderingRoot(View* window) const
{
    return window == renderingRoot;
}
