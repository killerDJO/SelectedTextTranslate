#include "Windows\Controls\Buttons\HoverIconButtonWindow.h"

map<tuple<DWORD, int, int>, HDC> HoverIconButtonWindow::iconsCache = map<tuple<DWORD, int, int>, HDC>();

HoverIconButtonWindow::HoverIconButtonWindow(WindowContext* context, WindowDescriptor descriptor, HWND parentWindow, DWORD normalIconResource, DWORD hoverIconResource, function<void()> clickCallback)
    : HoverButtonWindow(context, descriptor, parentWindow, clickCallback)
{
    this->normalIconResource = normalIconResource;
    this->hoverIconResource = hoverIconResource;
}

void HoverIconButtonWindow::RenderStatesDC()
{
    normalStateDC = context->GetDeviceContextProvider()->CreateInMemoryDC(windowSize);
    hoverStateDC = context->GetDeviceContextProvider()->CreateInMemoryDC(windowSize);

    RenderStateDC(normalStateDC, normalIconResource);
    RenderStateDC(hoverStateDC, hoverIconResource);
}

void HoverIconButtonWindow::RenderStateDC(HDC hdc, DWORD iconResource)
{
    tuple<int, int, int> cacheKey = tuple<int, int, int>(iconResource, windowSize.Width, windowSize.Height);

    if (iconsCache.count(cacheKey) != 0)
    {
        HDC renderedDC = iconsCache[cacheKey];
        context->GetDeviceContextProvider()->CopyDC(renderedDC, hdc, windowSize);
        return;
    }

    Graphics graphics(hdc);

    Renderer* renderer = context->GetRenderingContext()->GetRenderer(hdc);
    renderer->ClearDC(windowSize);
    context->GetRenderingContext()->ReleaseRenderer(renderer);

    Metafile* iconMetafile = LoadMetafileFromResource(iconResource);

    graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    graphics.SetSmoothingMode(SmoothingModeHighQuality);
    graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);

    BOOL converstionSucceded;
    iconMetafile->ConvertToEmfPlus(&graphics, &converstionSucceded);

    graphics.DrawImage(iconMetafile, 0, 0, windowSize.Width, windowSize.Height);

    delete iconMetafile;

    HDC cachedDC = context->GetDeviceContextProvider()->CreateInMemoryDC(windowSize);
    context->GetDeviceContextProvider()->CopyDC(hdc, cachedDC, windowSize);
    iconsCache[cacheKey] = cachedDC;
}

Metafile* HoverIconButtonWindow::LoadMetafileFromResource(DWORD resourceId) const
{
    HRSRC hResource = FindResource(context->GetInstance(), MAKEINTRESOURCE(resourceId), RT_RCDATA);
    if (!hResource)
    {
        return nullptr;
    }

    DWORD imageSize = SizeofResource(context->GetInstance(), hResource);
    if (!imageSize)
    {
        return nullptr;
    }

    const void* pResourceData = LockResource(LoadResource(context->GetInstance(), hResource));
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