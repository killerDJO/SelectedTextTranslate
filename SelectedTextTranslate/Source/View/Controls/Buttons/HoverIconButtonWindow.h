#pragma once
#include "View\Controls\Buttons\Base\HoverButtonWindow.h"

class HoverIconButtonWindow : public HoverButtonWindow
{
private:
    DWORD normalIconResource;
    DWORD hoverIconResource;

    void RenderStateDeviceContext(HDC deviceContext, DWORD iconResource);

    Metafile* LoadMetafileFromResource(DWORD resourceId) const;

    static map<tuple<DWORD, int, int>, HDC> iconsCache;

protected:
    void RenderStatesDeviceContext() override;

public:
    HoverIconButtonWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow, DWORD normalIconResource, DWORD hoverIconResource, function<void()> clickCallback);
    ~HoverIconButtonWindow();
};