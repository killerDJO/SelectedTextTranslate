#include "Presentation\Controls\Buttons\HoverIconButtonWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateException.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"

map<tuple<DWORD, int, int>, HDC> HoverIconButtonWindow::iconsCache = map<tuple<DWORD, int, int>, HDC>();

HoverIconButtonWindow::HoverIconButtonWindow(CommonContext* context, View* parentWindow)
    : HoverButtonWindow(context, parentWindow)
{
    this->hoverIconResource = 0;
    this->normalIconResource = 0;
    this->backgroundBrush = nullptr;
    this->className = L"STT_HOVERICONBUTTON";
}

void HoverIconButtonWindow::SetDimensions(Point position, Size size)
{
    AssertViewNotInitialized();
    layoutDescriptor = LayoutDescriptor::CreateFixedLayoutDescriptor(position, size);
}

void HoverIconButtonWindow::SetNormalIconResource(DWORD normalIconResource)
{
    AssertViewNotInitialized();
    this->normalIconResource = normalIconResource;
}

DWORD HoverIconButtonWindow::GetNormalIconResource() const
{
    return normalIconResource;
}

void HoverIconButtonWindow::SetHoverIconResource(DWORD hoverIconResource)
{
    AssertViewNotInitialized();
    this->hoverIconResource = hoverIconResource;
}

DWORD HoverIconButtonWindow::GetHoverIconResource() const
{
    return hoverIconResource;
}

void HoverIconButtonWindow::SetBackgroundBrush(Brush* backgroundBrush)
{
    AssertViewNotInitialized();
    this->backgroundBrush = backgroundBrush;
}

Brush* HoverIconButtonWindow::GetBackgroundBrush() const
{
    return backgroundBrush;
}

void HoverIconButtonWindow::Initialize()
{
    if(normalIconResource == 0 || hoverIconResource == 0)
    {
        throw new SelectedTextTranslateFatalException(L"Hover and normal icon resources must be set before initialization");
    }

    HoverButtonWindow::Initialize();
}

void HoverIconButtonWindow::RenderStatesDeviceContexts()
{
    stateToDeviceContextMap[ButtonStates::Normal] = deviceContextProvider->CreateDeviceContext(GetSize());
    stateToDeviceContextMap[ButtonStates::Hovered] = deviceContextProvider->CreateDeviceContext(GetSize());
    stateToDeviceContextMap[ButtonStates::Pressed] = stateToDeviceContextMap[ButtonStates::Hovered];

    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Normal], normalIconResource);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Hovered], hoverIconResource);
}

void HoverIconButtonWindow::RenderStateDeviceContext(HDC deviceContext, DWORD iconResource) const
{
    tuple<int, int, int> cacheKey = tuple<int, int, int>(iconResource, GetSize().GetWidth(), GetSize().GetHeight());

    if (iconsCache.count(cacheKey) != 0)
    {
        HDC renderedDC = iconsCache[cacheKey];
        deviceContextProvider->CopyDeviceContext(renderedDC, deviceContext, GetSize());
        return;
    }

    Gdiplus::Graphics graphics(deviceContext);

    Renderer* renderer = renderingContext->GetRenderer();

    if(backgroundBrush != nullptr)
    {
        renderer->SetBackground(backgroundBrush);
    }

    renderer->Render(deviceContext, deviceContextBuffer->GetSize());

    renderingContext->ReleaseRenderer(renderer);

    Gdiplus::Metafile* iconMetafile = LoadMetafileFromResource(iconResource);

    AssertGdiPlusResult(graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic));
    AssertGdiPlusResult(graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality));
    AssertGdiPlusResult(graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality));

    BOOL converstionSucceded;
    AssertGdiPlusResult(iconMetafile->ConvertToEmfPlus(&graphics, &converstionSucceded));

    AssertGdiPlusResult(graphics.DrawImage(iconMetafile, 0, 0, GetSize().GetWidth(), GetSize().GetHeight()));

    delete iconMetafile;

    HDC cachedDC = deviceContextProvider->CreateDeviceContext(GetSize());
    deviceContextProvider->CopyDeviceContext(deviceContext, cachedDC, GetSize());
    iconsCache[cacheKey] = cachedDC;
}

Gdiplus::Metafile* HoverIconButtonWindow::LoadMetafileFromResource(DWORD resourceId) const
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
    Gdiplus::Metafile* metafile;
    if (CreateStreamOnHGlobal(globalBuffer, FALSE, &pStream) == S_OK)
    {
        metafile = new Gdiplus::Metafile(pStream);
        pStream->Release();
    }
    else
    {
        throw SelectedTextTranslateException(L"Unable to CreateStreamOnHGlobal");
    }

    GlobalUnlock(globalBuffer);
    GlobalFree(globalBuffer);

    return metafile;
}