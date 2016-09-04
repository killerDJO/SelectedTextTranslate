#include "Windows\Buttons\HoverIconButtonWindow.h"

HoverIconButtonWindow::HoverIconButtonWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height, DWORD normalIconResource, DWORD hoverIconResource, function<void()> clickCallback)
    : HoverButtonWindow(parentWindow, hInstance, x, y, width, height, clickCallback)
{
    this->normalIconResource = normalIconResource;
    this->hoverIconResource = hoverIconResource;
}

void HoverIconButtonWindow::RenderStatesDC()
{
    normalStateDC = CreateInMemoryDC(width, height);
    hoverStateDC = CreateInMemoryDC(width, height);

    RenderStateDC(normalStateDC, normalIconResource);
    RenderStateDC(hoverStateDC, hoverIconResource);
}

void HoverIconButtonWindow::RenderStateDC(HDC hdc, DWORD iconResource)
{
    HBITMAP icon = LoadBitmap(hInstance, MAKEINTRESOURCE(iconResource));
    SelectObject(hdc, icon);
}

HoverIconButtonWindow::~HoverIconButtonWindow()
{
}