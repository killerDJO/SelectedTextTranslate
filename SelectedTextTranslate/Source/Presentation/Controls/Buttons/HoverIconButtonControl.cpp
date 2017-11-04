#include "Presentation\Controls\Buttons\HoverIconButtonControl.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateException.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"

map<tuple<DWORD, int, int>, HDC> HoverIconButtonControl::iconsCache = map<tuple<DWORD, int, int>, HDC>();

HoverIconButtonControl::HoverIconButtonControl(ViewContext* context, View* parentWindow)
    : HoverButtonControl(context, parentWindow)
{
    hoverIconResource = 0;
    normalIconResource = 0;
    backgroundBrush = nullptr;

    ClassName = L"STT_HOVERICONBUTTON";
}

HoverIconButtonControl* HoverIconButtonControl::SetDimensions(Point position, Size size)
{
    AssertViewNotInitialized();
    State->SetLayout(LayoutDescriptor::CreateFixedLayoutDescriptor(position, size));
    return this;
}

HoverIconButtonControl* HoverIconButtonControl::SetNormalIconResource(DWORD normalIconResource)
{
    AssertViewNotInitialized();
    this->normalIconResource = normalIconResource;
    return this;
}

DWORD HoverIconButtonControl::GetNormalIconResource() const
{
    return normalIconResource;
}

HoverIconButtonControl* HoverIconButtonControl::SetHoverIconResource(DWORD hoverIconResource)
{
    AssertViewNotInitialized();
    this->hoverIconResource = hoverIconResource;
    return this;
}

DWORD HoverIconButtonControl::GetHoverIconResource() const
{
    return hoverIconResource;
}

HoverIconButtonControl* HoverIconButtonControl::SetBackgroundBrush(Brush* backgroundBrush)
{
    AssertViewNotInitialized();
    this->backgroundBrush = backgroundBrush;
    return this;
}

Brush* HoverIconButtonControl::GetBackgroundBrush() const
{
    return backgroundBrush;
}

void HoverIconButtonControl::Initialize()
{
    if(normalIconResource == 0 || hoverIconResource == 0)
    {
        throw new SelectedTextTranslateFatalException(L"Hover and normal icon resources must be set before initialization");
    }

    HoverButtonControl::Initialize();
}

void HoverIconButtonControl::RenderStatesDeviceContexts()
{
    stateToDeviceContextMap[ButtonStates::Normal] = DeviceContextProvider->CreateDeviceContext(State->GetViewSize());
    stateToDeviceContextMap[ButtonStates::Hovered] = DeviceContextProvider->CreateDeviceContext(State->GetViewSize());
    stateToDeviceContextMap[ButtonStates::Pressed] = stateToDeviceContextMap[ButtonStates::Hovered];

    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Normal], normalIconResource);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Hovered], hoverIconResource);
}

void HoverIconButtonControl::RenderStateDeviceContext(HDC deviceContext, DWORD iconResource) const
{
    tuple<int, int, int> cacheKey = tuple<int, int, int>(iconResource, State->GetViewSize().GetWidth(), State->GetViewSize().GetHeight());

    if (iconsCache.count(cacheKey) != 0)
    {
        HDC renderedDC = iconsCache[cacheKey];
        DeviceContextProvider->CopyDeviceContext(renderedDC, deviceContext, State->GetViewSize());
        return;
    }

    Gdiplus::Graphics graphics(deviceContext);

    Renderer* renderer = RenderingContext->GetRenderer();

    if(backgroundBrush != nullptr)
    {
        renderer->SetBackground(backgroundBrush);
    }

    renderer->Render(deviceContext, State->GetViewSize());

    RenderingContext->ReleaseRenderer(renderer);

    Gdiplus::Metafile* iconMetafile = LoadMetafileFromResource(iconResource);

    AssertGdiPlusResult(graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic));
    AssertGdiPlusResult(graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality));
    AssertGdiPlusResult(graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality));

    BOOL converstionSucceded;
    AssertGdiPlusResult(iconMetafile->ConvertToEmfPlus(&graphics, &converstionSucceded));

    AssertGdiPlusResult(graphics.DrawImage(iconMetafile, 0, 0, State->GetViewSize().GetWidth(), State->GetViewSize().GetHeight()));

    delete iconMetafile;

    HDC cachedDC = DeviceContextProvider->CreateDeviceContext(State->GetViewSize());
    DeviceContextProvider->CopyDeviceContext(deviceContext, cachedDC, State->GetViewSize());
    iconsCache[cacheKey] = cachedDC;
}

Gdiplus::Metafile* HoverIconButtonControl::LoadMetafileFromResource(DWORD resourceId) const
{
    HRSRC hResource = FindResource(Instance, MAKEINTRESOURCE(resourceId), RT_RCDATA);
    AssertCriticalWinApiResult(hResource);

    DWORD imageSize = SizeofResource(Instance, hResource);
    AssertCriticalWinApiResult(imageSize);

    void* pResourceData = LockResource(LoadResource(Instance, hResource));
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