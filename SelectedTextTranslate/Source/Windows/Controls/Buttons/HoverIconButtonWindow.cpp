#include "Windows\Controls\Buttons\HoverIconButtonWindow.h"

map<tuple<DWORD, int, int>, HDC> HoverIconButtonWindow::iconsCache = map<tuple<DWORD, int, int>, HDC>();

HoverIconButtonWindow::HoverIconButtonWindow(HINSTANCE hInstance, RenderingContext* renderingContext, ScrollProvider* scrollProvider, WindowDescriptor descriptor, HWND parentWindow, DWORD normalIconResource, DWORD hoverIconResource, function<void()> clickCallback)
    : HoverButtonWindow(hInstance, renderingContext, scrollProvider, descriptor, parentWindow, clickCallback)
{
    this->normalIconResource = normalIconResource;
    this->hoverIconResource = hoverIconResource;
}

void HoverIconButtonWindow::RenderStatesDC()
{
    normalStateDC = renderingContext->CreateInMemoryDC(windowSize);
    hoverStateDC = renderingContext->CreateInMemoryDC(windowSize);

    RenderStateDC(normalStateDC, normalIconResource);
    RenderStateDC(hoverStateDC, hoverIconResource);
}

void HoverIconButtonWindow::RenderStateDC(HDC hdc, DWORD iconResource)
{
    tuple<int, int, int> cacheKey = tuple<int, int, int>(iconResource, windowSize.Width, windowSize.Height);

    if (iconsCache.count(cacheKey) != 0)
    {
        HDC renderedDC = iconsCache[cacheKey];
        renderingContext->CopyDC(renderedDC, hdc, windowSize);
        return;
    }

    Graphics graphics(hdc);

    Renderer* renderer = new Renderer(hdc, renderingContext);
    renderer->ClearDC(hdc, windowSize);
    delete renderer;

    Metafile* iconMetafile = LoadMetafileFromResource(iconResource);

    graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    graphics.SetSmoothingMode(SmoothingModeHighQuality);
    graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);

    BOOL converstionSucceded;
    iconMetafile->ConvertToEmfPlus(&graphics, &converstionSucceded);

    graphics.DrawImage(iconMetafile, 0, 0, windowSize.Width, windowSize.Height);

    delete iconMetafile;

    HDC cachedDC = renderingContext->CreateInMemoryDC(windowSize);
    renderingContext->CopyDC(hdc, cachedDC, windowSize);
    iconsCache[cacheKey] = cachedDC;
}

Metafile* HoverIconButtonWindow::LoadMetafileFromResource(DWORD resourceId) const
{
    HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(resourceId), RT_RCDATA);
    if (!hResource)
    {
        return nullptr;
    }

    DWORD imageSize = SizeofResource(hInstance, hResource);
    if (!imageSize)
    {
        return nullptr;
    }

    const void* pResourceData = LockResource(LoadResource(hInstance, hResource));
    if (!pResourceData)
    {
        return nullptr;
    }

    HGLOBAL globalBuffer = GlobalAlloc(GMEM_MOVEABLE, imageSize);
    if (globalBuffer)
    {
        void* pBuffer = GlobalLock(globalBuffer);
        if (pBuffer)
        {
            CopyMemory(pBuffer, pResourceData, imageSize);

            IStream* pStream = nullptr;
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

    return nullptr;
}

HoverIconButtonWindow::~HoverIconButtonWindow()
{
}