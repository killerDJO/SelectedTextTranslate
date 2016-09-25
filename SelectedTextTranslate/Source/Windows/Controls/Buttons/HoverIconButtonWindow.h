#pragma once
#include "Windows\Controls\Buttons\Base\HoverButtonWindow.h"

class HoverIconButtonWindow : public HoverButtonWindow
{
private:
    DWORD normalIconResource;
    DWORD hoverIconResource;

    void RenderStateDC(HDC hdc, DWORD iconResource);
    Metafile* LoadMetafileFromResource(DWORD resourceId) const;

    static map<tuple<DWORD, int, int>, HDC> iconsCache;

protected:
    void RenderStatesDC() override;

public:
    HoverIconButtonWindow(WindowContext* context, WindowDescriptor descriptor, HWND parentWindow, DWORD normalIconResource, DWORD hoverIconResource, function<void()> clickCallback);
    ~HoverIconButtonWindow();
};