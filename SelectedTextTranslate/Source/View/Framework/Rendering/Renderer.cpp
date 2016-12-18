#include "View\Framework\Rendering\Renderer.h"
#include "Utilities\StringUtilities.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "View\Framework\Dto\Point\PointReal.h"

Renderer::Renderer(RenderingContext* renderingContext, ScaleProvider* scaleProvider)
{
    this->renderingContext = renderingContext;
    this->scaleProvider = scaleProvider;
    this->originalSize = Size(0, 0);
    this->backgroundBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    this->renderActions = vector<function<void(HDC)>>();
}

TextRenderResult Renderer::PrintText(wstring text, HFONT font, Colors color, RenderPosition renderPosition, DWORD horizontalAlignment)
{
    PointReal position = renderPosition.GetPosition();
    Point scaledPosition = scaleProvider->Scale(position);

    // Copy text to prevent preliminary dispose by calling code.
    wchar_t* copiedText = StringUtilities::CopyWideChar(text);
    auto printTextAction = [=](HDC deviceContext) -> void {
        AssertCriticalWinApiResult(SelectObject(deviceContext, font));
        ExceptionHelper::ThrowOnWinapiError(SetTextColor(deviceContext, (COLORREF)color), true, CLR_INVALID);
        ExceptionHelper::ThrowOnWinapiError(SetTextAlign(deviceContext, TA_BASELINE | horizontalAlignment), true, GDI_ERROR);
        AssertCriticalWinApiResult(SetBkMode(deviceContext, TRANSPARENT));

        AssertCriticalWinApiResult(TextOut(deviceContext, scaledPosition.GetX(), scaledPosition.GetY(), copiedText, wcslen(copiedText)));

        delete copiedText;
    };
    renderActions.push_back(printTextAction);

    Size textSize = renderingContext->GetTextSize(text.c_str(), font);
    int fontAscent = renderingContext->GetFontAscent(font);

    originalSize = Size(
        max(originalSize.GetWidth(), scaledPosition.GetX() + textSize.GetWidth()),
        max(originalSize.GetHeight(), scaledPosition.GetY() + textSize.GetHeight() - fontAscent)
    );

    SizeReal downscaledSize = scaleProvider->Downscale(textSize);

    double rightX = position.GetX() + downscaledSize.GetWidth();
    double bottomY = position.GetY() - scaleProvider->Downscale(fontAscent) + downscaledSize.GetHeight();

    return TextRenderResult(downscaledSize, rightX, position.GetY(), bottomY);
}

void Renderer::DrawRect(RectReal rect, HBRUSH brush)
{
    Rect scaledRect = scaleProvider->Scale(rect);

    RECT gdiRect;
    gdiRect.left = scaledRect.GetX();
    gdiRect.right = scaledRect.GetRight();
    gdiRect.top = scaledRect.GetY();
    gdiRect.bottom = scaledRect.GetBottom();

    auto drawRectAction = [=](HDC hdc) -> void {
        AssertCriticalWinApiResult(FillRect(hdc, &gdiRect, brush));
    };
    renderActions.push_back(drawRectAction);

    originalSize = Size(
        max(originalSize.GetWidth(), scaledRect.GetWidth()),
        max(originalSize.GetHeight(), scaledRect.GetHeight())
    );
}

void Renderer::DrawBorderedRect(RectReal rect, HBRUSH brush, double borderWidth, Colors borderColor)
{
    int scaledBorderWidth = scaleProvider->Scale(borderWidth);
    Rect scaledRect = scaleProvider->Scale(rect);

    int borderAjustments = scaledBorderWidth / 2;
    scaledRect = Rect(
        scaledRect.GetX() + borderAjustments,
        scaledRect.GetY() + borderAjustments,
        scaledRect.GetWidth() - borderAjustments,
        scaledRect.GetHeight() - borderAjustments
    );

    HPEN borderPen = renderingContext->CreateCustomPen(borderColor, scaledBorderWidth);
    HBRUSH contentBrush = brush != nullptr
        ? brush
        : (HBRUSH)GetStockObject(HOLLOW_BRUSH);

    auto drawRectAction = [=](HDC hdc) -> void {
        AssertCriticalWinApiResult(SelectObject(hdc, borderPen));
        AssertCriticalWinApiResult(SelectObject(hdc, contentBrush));

        AssertCriticalWinApiResult(Rectangle(hdc, scaledRect.GetX(), scaledRect.GetY(), scaledRect.GetRight(), scaledRect.GetBottom()));

        AssertCriticalWinApiResult(DeleteObject(borderPen));
    };
    renderActions.push_back(drawRectAction);

    originalSize = Size(
        max(originalSize.GetWidth(), scaledRect.GetWidth()),
        max(originalSize.GetHeight(), scaledRect.GetHeight())
    );
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
    rect.bottom = deviceContextSize.GetHeight();
    rect.right = deviceContextSize.GetWidth();

    AssertCriticalWinApiResult(FillRect(deviceContext, &rect, backgroundBrush));
}

double Renderer::GetFontAscent(HFONT font) const
{
    return scaleProvider->Downscale(renderingContext->GetFontAscent(font));
}

double Renderer::GetFontDescent(HFONT font) const
{
    return scaleProvider->Downscale(renderingContext->GetFontDescent(font));
}

double Renderer::GetFontStrokeHeight(HFONT font) const
{
    return scaleProvider->Downscale(renderingContext->GetFontStrokeHeight(font));
}

double Renderer::GetFontHeight(HFONT font) const
{
    return scaleProvider->Downscale(renderingContext->GetFontHeight(font));
}

Size Renderer::GetScaledSize() const
{
    return originalSize;
}

SizeReal Renderer::GetSize() const
{
    return scaleProvider->Downscale(originalSize);
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

void Renderer::IncreaseWidth(double widthToAdd)
{
    originalSize = Size(
        originalSize.GetWidth() + scaleProvider->Scale(widthToAdd),
        originalSize.GetHeight());
}

void Renderer::IncreaseHeight(double heightToAdd)
{
    originalSize = Size(
        originalSize.GetWidth(),
        originalSize.GetHeight() + scaleProvider->Scale(heightToAdd));
}

void Renderer::UpdateRenderedContentSize(Window* window)
{
    Rect windowRect = window->GetBoundingRect();
    originalSize = Size(
        max(originalSize.GetWidth(), windowRect.GetRight()),
        max(originalSize.GetHeight(), windowRect.GetBottom())
    );
}

Renderer::~Renderer()
{
    renderActions.clear();
}
