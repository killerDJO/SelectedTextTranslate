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
    HoverIconButtonWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height, DWORD normalIconResource, DWORD hoverIconResource, function<void()> clickCallback);
    ~HoverIconButtonWindow();
};