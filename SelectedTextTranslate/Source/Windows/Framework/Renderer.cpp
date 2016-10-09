#include "Windows\Framework\Renderer.h"
#include "Helpers\StringUtilities.h"

Renderer::Renderer(RenderingContext* renderingContext, DeviceContextProvider* deviceContextProvider, ScaleProvider* scaleProvider)
{
    this->emptyDeviceContext = deviceContextProvider->CreateDeviceContext(Size(0, 0));
    this->renderingContext = renderingContext;
    this->scaleProvider = scaleProvider;
    this->scaledSize = Size(0, 0);
    renderActions = vector<function<void(HDC)>>();
}

Point Renderer::PrintText(const wchar_t* text, HFONT font, Colors color, Point position)
{
    Point scaledPosition = scaleProvider->Scale(position);

    // Copy text to prevent preliminary dispose by calling code.
    wchar_t* copiedText = StringUtilities::CopyWideChar(text);
    auto printTextAction = [=](HDC deviceContext) -> void {
        SelectObject(deviceContext, font);
        SetTextColor(deviceContext, (COLORREF)color);

        SetTextAlign(deviceContext, TA_BASELINE | TA_LEFT);
        TextOut(deviceContext, scaledPosition.X, scaledPosition.Y, copiedText, wcslen(copiedText));
        delete copiedText;
    };
    renderActions.push_back(printTextAction);

    Size textSize = renderingContext->GetTextSize(emptyDeviceContext, text, font);

    scaledSize.Width = max(scaledSize.Width, scaledPosition.X + textSize.Width);
    scaledSize.Height = max(scaledSize.Height, scaledPosition.Y + textSize.Height);

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
        FillRect(hdc, &gdiRect, brush);
    };
    renderActions.push_back(drawRectAction);

    scaledSize.Width = max(scaledSize.Width, scaledRect.Width);
    scaledSize.Height = max(scaledSize.Height, scaledRect.Height);
}

void Renderer::ClearDeviceContext(HDC deviceContext, Size deviceContextSize) const
{
    RECT rect;
    rect.top = 0;
    rect.left = 0;
    rect.bottom = deviceContextSize.Height;
    rect.right = deviceContextSize.Width;

    FillRect(deviceContext, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
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

Size Renderer::GetScaledSize() const
{
    return scaledSize;
}

Size Renderer::GetSize() const
{
    return scaleProvider->Downscale(scaledSize);
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
    scaledSize.Width += scaleProvider->Scale(widthToAdd);
}

void Renderer::IncreaseHeight(int heightToAdd)
{
    scaledSize.Height += scaleProvider->Scale(heightToAdd);
}

Renderer::~Renderer()
{
    renderActions.clear();
}