#include "View\Controls\Buttons\HoverIconButtonWindow.h"
#include "Exceptions\SelectedTextTranslateFatalException.h"
#include "Helpers\ExceptionHelper.h"

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

    AssertGdiPlusResult(graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic));
    AssertGdiPlusResult(graphics.SetSmoothingMode(SmoothingModeHighQuality));
    AssertGdiPlusResult(graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality));

    BOOL converstionSucceded;
    AssertGdiPlusResult(iconMetafile->ConvertToEmfPlus(&graphics, &converstionSucceded));

    AssertGdiPlusResult(graphics.DrawImage(iconMetafile, 0, 0, windowSize.Width, windowSize.Height));

    delete iconMetafile;

    HDC cachedDC = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);
    context->GetDeviceContextProvider()->CopyDeviceContext(deviceContext, cachedDC, windowSize);
    iconsCache[cacheKey] = cachedDC;
}

Metafile* HoverIconButtonWindow::LoadMetafileFromResource(DWORD resourceId) const
{
    HRSRC hResource = FindResource(context->GetInstance(), MAKEINTRESOURCE(resourceId), RT_RCDATA);
    AssertCriticalWinApiResult(hResource);

    DWORD imageSize = SizeofResource(context->GetInstance(), hResource);
    AssertCriticalWinApiResult(imageSize);

    void* pResourceData = LockResource(LoadResource(context->GetInstance(), hResource));
    AssertCriticalWinApiResult(pResourceData);

    HGLOBAL globalBuffer = GlobalAlloc(GMEM_MOVEABLE, imageSize);
    AssertCriticalWinApiResult(globalBuffer);

    void* pBuffer = GlobalLock(globalBuffer);
    AssertCriticalWinApiResult(pBuffer);

    CopyMemory(pBuffer, pResourceData, imageSize);

    IStream* pStream = nullptr;
    Metafile* metafile;
    if (CreateStreamOnHGlobal(globalBuffer, FALSE, &pStream) == S_OK)
    {
        metafile = new Metafile(pStream);
        pStream->Release();
    }
    else
    {
        throw SelectedTextTranslateFatalException(L"Unable to CreateStreamOnHGlobal", __WFILE__, __LINE__);
    }

    GlobalUnlock(globalBuffer);
    GlobalFree(globalBuffer);

    return metafile;
}

HoverIconButtonWindow::~HoverIconButtonWindow()
{
}