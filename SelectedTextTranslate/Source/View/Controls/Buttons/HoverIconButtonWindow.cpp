#include "View\Controls\Buttons\HoverIconButtonWindow.h"

map<tuple<DWORD, int, int>, HDC> HoverIconButtonWindow::iconsCache = map<tuple<DWORD, int, int>, HDC>();

HoverIconButtonWindow::HoverIconButtonWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow, DWORD normalIconResource, DWORD hoverIconResource, function<void()> clickCallback)
    : HoverButtonWindow(context, descriptor, parentWindow, clickCallback)
{
    this->normalIconResource = normalIconResource;
    this->hoverIconResource = hoverIconResource;
}

void HoverIconButtonWindow::RenderStatesDeviceContext()
{
    normalStateDeviceContext = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);
    hoverStateDeviceContext = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);

    RenderStateDeviceContext(normalStateDeviceContext, normalIconResource);
    RenderStateDeviceContext(hoverStateDeviceContext, hoverIconResource);
}

void HoverIconButtonWindow::RenderStateDeviceContext(HDC deviceContext, DWORD iconResource)
{
    tuple<int, int, int> cacheKey = tuple<int, int, int>(iconResource, windowSize.Width, windowSize.Height);

    if (iconsCache.count(cacheKey) != 0)
    {
        HDC renderedDC = iconsCache[cacheKey];
        context->GetDeviceContextProvider()->CopyDeviceContext(renderedDC, deviceContext, windowSize);
        return;
    }

    Graphics graphics(deviceContext);

    Renderer* renderer = context->GetRenderingContext()->GetRenderer();
    renderer->Render(deviceContext, deviceContextBuffer->GetSize());
    context->GetRenderingContext()->ReleaseRenderer(renderer);

    Metafile* iconMetafile = LoadMetafileFromResource(iconResource);

    graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    graphics.SetSmoothingMode(SmoothingModeHighQuality);
    graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);

    BOOL converstionSucceded;
    iconMetafile->ConvertToEmfPlus(&graphics, &converstionSucceded);

    graphics.DrawImage(iconMetafile, 0, 0, windowSize.Width, windowSize.Height);

    delete iconMetafile;

    HDC cachedDC = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);
    context->GetDeviceContextProvider()->CopyDeviceContext(deviceContext, cachedDC, windowSize);
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