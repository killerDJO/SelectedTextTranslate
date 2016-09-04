#include "Windows\Buttons\HoverIconButtonWindow.h"

HoverIconButtonWindow::HoverIconButtonWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height, DWORD normalIconResource, DWORD hoverIconResource, function<void()> clickCallback)
    : HoverButtonWindow(parentWindow, hInstance, x, y, width, height, clickCallback)
{
    this->normalIconResource = normalIconResource;
    this->hoverIconResource = hoverIconResource;
}

void HoverIconButtonWindow::RenderStatesHDC()
{
    this->normalStateHDC = CreateInMemoryHDC(this->width, this->height);
    this->hoverStateHDC = CreateInMemoryHDC(this->width, this->height);

    RenderStateHDC(this->normalStateHDC, this->normalIconResource);
    RenderStateHDC(this->hoverStateHDC, this->hoverIconResource);
}

void HoverIconButtonWindow::RenderStateHDC(HDC hdc, DWORD iconResource)
{
    HBITMAP icon = LoadBitmap(hInstance, MAKEINTRESOURCE(iconResource));
    SelectObject(hdc, icon);
}

HoverIconButtonWindow::~HoverIconButtonWindow()
{
}