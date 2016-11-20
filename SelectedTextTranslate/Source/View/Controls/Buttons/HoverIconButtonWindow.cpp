#include "View\Controls\Buttons\HoverIconButtonWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateException.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"

map<tuple<DWORD, int, int>, HDC> HoverIconButtonWindow::iconsCache = map<tuple<DWORD, int, int>, HDC>();

HoverIconButtonWindow::HoverIconButtonWindow(WindowContext* context, Window* parentWindow)
    : HoverButtonWindow(context, parentWindow)
{
    this->hoverIconResource = 0;
    this->normalIconResource = 0;
    this->backgroundBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
}

void HoverIconButtonWindow::SetDescriptor(WindowDescriptor descriptor)
{
    throw new SelectedTextTranslateFatalException(L"SetDescriptor is unsupported");
}

void HoverIconButtonWindow::SetDimensions(Point position, Size size)
{
    AssertWindowNotInitialized();
    descriptor = WindowDescriptor::CreateFixedWindowDescriptor(position, size);
}

void HoverIconButtonWindow::SetNormalIconResource(DWORD normalIconResource)
{
    AssertWindowNotInitialized();
    this->normalIconResource = normalIconResource;
}

DWORD HoverIconButtonWindow::GetNormalIconResource() const
{
    return normalIconResource;
}

void HoverIconButtonWindow::SetHoverIconResource(DWORD hoverIconResource)
{
    AssertWindowNotInitialized();
    this->hoverIconResource = hoverIconResource;
}

DWORD HoverIconButtonWindow::GetHoverIconResource() const
{
    return hoverIconResource;
}

void HoverIconButtonWindow::SetBackgroundBrush(HBRUSH backgroundBrush)
{
    AssertWindowNotInitialized();
    this->backgroundBrush = backgroundBrush;
}

HBRUSH HoverIconButtonWindow::GetBackgroundBrush() const
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

void HoverIconButtonWindow::RenderStatesDeviceContext()
{
    stateToDeviceContextMap[ButtonStates::Normal] = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);
    stateToDeviceContextMap[ButtonStates::Hovered] = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);
    stateToDeviceContextMap[ButtonStates::Pressed] = stateToDeviceContextMap[ButtonStates::Hovered];

    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Normal], normalIconResource);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Hovered], hoverIconResource);
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
    
    if(backgroundBrush != nullptr)
    {
        renderer->SetBackground(backgroundBrush);
    }

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
        throw SelectedTextTranslateException(L"Unable to CreateStreamOnHGlobal");
    }

    GlobalUnlock(globalBuffer);
    GlobalFree(globalBuffer);

    return metafile;
}

HoverIconButtonWindow::~HoverIconButtonWindow()
{
}