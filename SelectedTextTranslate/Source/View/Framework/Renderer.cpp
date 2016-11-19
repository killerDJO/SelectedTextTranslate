#include "View\Framework\Renderer.h"
#include "Utilities\StringUtilities.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

Renderer::Renderer(RenderingContext* renderingContext, DeviceContextProvider* deviceContextProvider, ScaleProvider* scaleProvider, ScrollProvider* scrollProvider)
{
    this->emptyDeviceContext = deviceContextProvider->CreateDeviceContext(Size(0, 0));
    this->renderingContext = renderingContext;
    this->scaleProvider = scaleProvider;
    this->scrollProvider = scrollProvider;
    this->originalSize = Size(0, 0);
    this->backgroundBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    this->renderActions = vector<function<void(HDC)>>();
}

Point Renderer::PrintText(wstring text, HFONT font, Colors color, Point position, DWORD horizontalAlignment)
{
    Point scaledPosition = scaleProvider->Scale(position);

    // Copy text to prevent preliminary dispose by calling code.
    wchar_t* copiedText = StringUtilities::CopyWideChar(text);
    auto printTextAction = [=](HDC deviceContext) -> void {
        AssertCriticalWinApiResult(SelectObject(deviceContext, font));
        ExceptionHelper::ThrowOnWinapiError(SetTextColor(deviceContext, (COLORREF)color), true, CLR_INVALID);
        ExceptionHelper::ThrowOnWinapiError(SetTextAlign(deviceContext, TA_BASELINE | horizontalAlignment), true, GDI_ERROR);
        SetBkMode(deviceContext, TRANSPARENT);

        AssertCriticalWinApiResult(TextOut(deviceContext, scaledPosition.X, scaledPosition.Y, copiedText, wcslen(copiedText)));

        delete copiedText;
    };
    renderActions.push_back(printTextAction);

    Size textSize = renderingContext->GetTextSize(emptyDeviceContext, text.c_str(), font);

    originalSize.Width = max(originalSize.Width, scaledPosition.X + textSize.Width);
    originalSize.Height = max(originalSize.Height, scaledPosition.Y + textSize.Height);

    Size downscaledSize = scaleProvider->Downscale(textSize);

    Point result;
    result.X = position.X + downscaledSize.Width;
    result.Y = position.Y - GetFontAscent(font) + downscaledSize.Height;

    return result;
}

void Renderer::DrawRect(Rect rect, HBRUSH brush)
{
    Rect scaledRect = scaleProvider->Scale(rect);

    RECT gdiRect;
    gdiRect.left = scaledRect.X;
    gdiRect.right = scaledRect.GetRight();
    gdiRect.top = scaledRect.Y;
    gdiRect.bottom = scaledRect.GetBottom();

    auto drawRectAction = [=](HDC hdc) -> void {
        AssertCriticalWinApiResult(FillRect(hdc, &gdiRect, brush));
    };
    renderActions.push_back(drawRectAction);

    originalSize.Width = max(originalSize.Width, scaledRect.Width);
    originalSize.Height = max(originalSize.Height, scaledRect.Height);
}

void Renderer::DrawBorderedRect(Rect rect, HBRUSH brush, int borderWidth, Colors borderColor)
{
    int scaledBorderWidth = scaleProvider->Scale(borderWidth);
    Rect scaledRect = scaleProvider->Scale(rect);

    int borderAjustments = scaledBorderWidth / 2;
    scaledRect.X = scaledRect.X + borderAjustments;
    scaledRect.Y = scaledRect.Y + borderAjustments;
    scaledRect.Width = scaledRect.Width - borderAjustments;
    scaledRect.Height = scaledRect.Height - borderAjustments;

    HPEN borderPen = renderingContext->CreateCustomPen(borderColor, borderWidth);

    auto drawRectAction = [=](HDC hdc) -> void {
        AssertCriticalWinApiResult(SelectObject(hdc, borderPen));

        if(brush != nullptr)
        {
            AssertCriticalWinApiResult(SelectObject(hdc, brush));
        }

        AssertCriticalWinApiResult(Rectangle(hdc, scaledRect.X, scaledRect.Y, scaledRect.GetRight(), scaledRect.GetBottom()));
    };
    renderActions.push_back(drawRectAction);

    originalSize.Width = max(originalSize.Width, scaledRect.Width);
    originalSize.Height = max(originalSize.Height, scaledRect.Height);
}

void Renderer::SetBackground(HBRUSH backgroundBrush)
{
    this->backgroundBrush = backgroundBrush;
}

void Renderer::ClearDeviceContext(HDC deviceContext, Size deviceContextSize) const
{
    RECT rect;
    rect.top = 0;
    rect.left = 0;
    rect.bottom = deviceContextSize.Height;
    rect.right = deviceContextSize.Width;

    AssertCriticalWinApiResult(FillRect(deviceContext, &rect, backgroundBrush));
}

int Renderer::GetFontAscent(HFONT font) const
{
    return scaleProvider->Downscale(renderingContext->GetFontMetrics(emptyDeviceContext, font).tmAscent);
}

int Renderer::GetFontStrokeHeight(HFONT font) const
{
    TEXTMETRIC textMetrics = renderingContext->GetFontMetrics(emptyDeviceContext, font);
    return scaleProvider->Downscale(textMetrics.tmAscent - textMetrics.tmInternalLeading);
}

int Renderer::GetFontHeight(HFONT font) const
{
    return scaleProvider->Downscale(renderingContext->GetFontMetrics(emptyDeviceContext, font).tmHeight);
}

Size Renderer::GetScaledSize() const
{
    // Align content size with scrolling grid.
    int scrollCharX = scrollProvider->GetScrollChar(ScrollBars::Horizontal);
    int scrollCharY = scrollProvider->GetScrollChar(ScrollBars::Vertical);
    return Size(
        roundToInt(ceil(originalSize.Width * 1.0 / scrollCharX) * scrollCharX),
        roundToInt(ceil(originalSize.Height * 1.0 / scrollCharY) * scrollCharY));
}

Size Renderer::GetSize() const
{
    return scaleProvider->Downscale(GetScaledSize());
}

void Renderer::Render(HDC deviceContext, Size deviceContextSize)
{
    ClearDeviceContext(deviceContext, deviceContextSize);

    for (size_t i = 0; i < renderActions.size(); ++i)
    {
        renderActions[i](deviceContext);
    }
}

void Renderer::Render(DeviceContextBuffer* deviceContextBuffer)
{
    Render(deviceContextBuffer->GetDeviceContext(), deviceContextBuffer->GetSize());
}

void Renderer::IncreaseWidth(int widthToAdd)
{
    originalSize.Width += scaleProvider->Scale(widthToAdd);
}

void Renderer::IncreaseHeight(int heightToAdd)
{
    originalSize.Height += scaleProvider->Scale(heightToAdd);
}

void Renderer::UpdateSize(Size size)
{
    originalSize.Width = max(originalSize.Width, size.Width);
    originalSize.Height = max(originalSize.Height, size.Height);
}

Renderer::~Renderer()
{
    renderActions.clear();
}
