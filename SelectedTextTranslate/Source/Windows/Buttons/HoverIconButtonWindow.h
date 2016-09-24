#pragma once
#include "Windows\Buttons\Base\HoverButtonWindow.h"

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
    HoverIconButtonWindow(HINSTANCE hInstance, RenderingContext* renderingContext, ScrollProvider* scrollProvider, WindowDescriptor descriptor, HWND parentWindow, DWORD normalIconResource, DWORD hoverIconResource, function<void()> clickCallback);
    ~HoverIconButtonWindow();
};