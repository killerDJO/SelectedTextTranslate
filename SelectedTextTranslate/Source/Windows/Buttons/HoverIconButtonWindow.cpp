#include "Windows\Buttons\HoverIconButtonWindow.h"

HoverIconButtonWindow::HoverIconButtonWindow(Renderer* renderer, HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height, DWORD normalIconResource, DWORD hoverIconResource, function<void()> clickCallback)
    : HoverButtonWindow(renderer, parentWindow, hInstance, x, y, width, height, clickCallback)
{
    this->normalIconResource = normalIconResource;
    this->hoverIconResource = hoverIconResource;
}

void HoverIconButtonWindow::RenderStatesDC()
{
    normalStateDC = renderer->CreateInMemoryDC(width, height);
    hoverStateDC = renderer->CreateInMemoryDC(width, height);

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