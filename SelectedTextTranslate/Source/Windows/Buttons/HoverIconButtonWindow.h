#pragma once
#include "Windows\Buttons\Base\HoverButtonWindow.h"

class HoverIconButtonWindow : public HoverButtonWindow
{
protected:
    void RenderStatesDC() override;

private:
    DWORD normalIconResource;
    DWORD hoverIconResource;

    void RenderStateDC(HDC hdc, DWORD iconResource);

public:
    HoverIconButtonWindow(HINSTANCE hInstance, RenderingContext* renderingContext, ScrollProvider* scrollProvider, WindowDescriptor descriptor, HWND parentWindow, DWORD normalIconResource, DWORD hoverIconResource, function<void()> clickCallback);
    ~HoverIconButtonWindow();
};