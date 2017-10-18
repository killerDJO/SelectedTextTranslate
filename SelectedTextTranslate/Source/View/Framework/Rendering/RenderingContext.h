#pragma once
#include "View\Framework\View\Views\View.h"
#include "View\Framework\Rendering\RenderingProvider.h"

class Renderer;
class View;
class DeviceContextProvider;
class RenderingProvider;

class RenderingContext
{
private:
    DeviceContextProvider* deviceContextProvider;
    RenderingProvider* renderingProvider;

    View* renderingRoot;

public:
    RenderingContext(CompositionRoot* root);

    Renderer* GetRenderer() const;
    void ReleaseRenderer(Renderer* renderer) const;

    void BeginRender(View* window);
    void EndRender(View* window);
    bool IsRenderingInProgress() const;
    bool IsRenderingRoot(View* window) const;
};