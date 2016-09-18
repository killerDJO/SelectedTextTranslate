#include "Windows\Buttons\HoverIconButtonWindow.h"

map<tuple<DWORD, int, int>, HDC> HoverIconButtonWindow::iconsCache = map<tuple<DWORD, int, int>, HDC>();

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
    tuple<int, int, int> cacheKey = tuple<int, int, int>(iconResource, currentWidth, currentHeight);

    if (iconsCache.count(cacheKey) != 0)
    {
        HDC renderedDC = iconsCache[cacheKey];
        renderingContext->CopyDC(renderedDC, hdc, currentWidth, currentHeight);
        return;
    }

    Graphics graphics(hdc);
    renderingContext->ClearDC(hdc, currentWidth, currentHeight);

    Metafile* iconMetafile = LoadMetafileFromResource(iconResource);

    graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    graphics.SetSmoothingMode(SmoothingModeHighQuality);
    graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);

    BOOL converstionSucceded;
    iconMetafile->ConvertToEmfPlus(&graphics, &converstionSucceded);

    graphics.DrawImage(iconMetafile, 0, 0, currentWidth, currentHeight);

    delete iconMetafile;

    HDC cachedDC = renderingContext->CreateInMemoryDC(currentWidth, currentHeight);
    renderingContext->CopyDC(hdc, cachedDC, currentWidth, currentHeight);
    iconsCache[cacheKey] = cachedDC;
}

Metafile* HoverIconButtonWindow::LoadMetafileFromResource(DWORD resourceId)
{
    HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(resourceId), RT_RCDATA);
    if (!hResource)
    {
        return NULL;
    }

    DWORD imageSize = SizeofResource(hInstance, hResource);
    if (!imageSize)
    {
        return NULL;
    }

    const void* pResourceData = LockResource(LoadResource(hInstance, hResource));
    if (!pResourceData)
    {
        return NULL;
    }

    HGLOBAL globalBuffer = GlobalAlloc(GMEM_MOVEABLE, imageSize);
    if (globalBuffer)
    {
        void* pBuffer = GlobalLock(globalBuffer);
        if (pBuffer)
        {
            CopyMemory(pBuffer, pResourceData, imageSize);

            IStream* pStream = NULL;
            if (CreateStreamOnHGlobal(globalBuffer, FALSE, &pStream) == S_OK)
            {
                Metafile* metafile = new Metafile(pStream);
                pStream->Release();

                return metafile;
            }

            GlobalUnlock(globalBuffer);
        }

        GlobalFree(globalBuffer);
    }

    return NULL;
}

HoverIconButtonWindow::~HoverIconButtonWindow()
{
}