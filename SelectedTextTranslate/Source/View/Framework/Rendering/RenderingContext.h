#pragma once
#include "View\Framework\Windows\Window.h"
#include "View\Framework\Rendering\RenderingProvider.h"

class Renderer;
class Window;
class DeviceContextProvider;
class RenderingProvider;

class RenderingContext
{
private:
    DeviceContextProvider* deviceContextProvider;
    RenderingProvider* renderingProvider;

    Window* renderingRoot;

public:
    RenderingContext(RenderingProvider* renderingProvider, DeviceContextProvider* deviceContextProvider);

    Renderer* GetRenderer() const;
    void ReleaseRenderer(Renderer* renderer) const;

    void BeginRender(Window* window);
    void EndRender(Window* window);
    bool IsRenderingRoot(Window* window) const;
};