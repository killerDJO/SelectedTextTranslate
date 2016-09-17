#include "Windows\Buttons\HoverIconButtonWindow.h"

HoverIconButtonWindow::HoverIconButtonWindow(HINSTANCE hInstance, RenderingContext* renderingContext, ScrollProvider* scrollProvider, WindowDescriptor descriptor, HWND parentWindow, DWORD normalIconResource, DWORD hoverIconResource, function<void()> clickCallback)
    : HoverButtonWindow(hInstance, renderingContext, scrollProvider, descriptor, parentWindow, clickCallback)
{
    this->normalIconResource = normalIconResource;
    this->hoverIconResource = hoverIconResource;
}

void HoverIconButtonWindow::RenderStatesDC()
{
    normalStateDC = renderingContext->CreateInMemoryDC(currentWidth, currentHeight);
    hoverStateDC = renderingContext->CreateInMemoryDC(currentWidth, currentHeight);

    RenderStateDC(normalStateDC, normalIconResource);
    RenderStateDC(hoverStateDC, hoverIconResource);
}

void HoverIconButtonWindow::RenderStateDC(HDC hdc, DWORD iconResource)
{
    HBITMAP icon = LoadBitmap(hInstance, MAKEINTRESOURCE(iconResource));

    BITMAP iconInfo;
    GetObject(icon, sizeof(BITMAP), &iconInfo);

    HDC bitmapDC = renderingContext->CreateInMemoryDC(iconInfo.bmWidth, iconInfo.bmHeight);
    SelectObject(bitmapDC, icon);

    //SetStretchBltMode(hDC, BLACKONWHITE);
    StretchBlt(hdc, 0, 0, currentWidth, currentHeight, bitmapDC, 0, 0, iconInfo.bmWidth, iconInfo.bmHeight, SRCCOPY);

    DeleteDC(bitmapDC);
}

HoverIconButtonWindow::~HoverIconButtonWindow()
{
}