#include "View\Framework\Rendering\RenderingContext.h"

RenderingContext::RenderingContext(CompositionRoot* root)
{
    this->deviceContextProvider = root->GetService<DeviceContextProvider>();
    this->renderingProvider = root->GetService<RenderingProvider>();

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
