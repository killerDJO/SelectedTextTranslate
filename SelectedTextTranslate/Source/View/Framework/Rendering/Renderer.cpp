#include "View\Framework\Rendering\Renderer.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Utilities\StringUtilities.h"
#include "View\Framework\Rendering\Objects\Brush.h"
#include "View\Framework\Rendering\Objects\Font.h"

Renderer::Renderer(RenderingProvider* renderingProvider, DeviceContextProvider* deviceContextProvider)
{
    this->renderingProvider = renderingProvider;
    this->deviceContextProvider = deviceContextProvider;
    this->renderedSize = Size(0, 0);
    this->defaultBackgroundBrush = renderingProvider->CreateCustomBrush(Colors::White);
    this->backgroundBrush = nullptr;
    this->renderActions = vector<function<void(HDC)>>();
}

TextRenderResult Renderer::PrintText(wstring text, Font* font, Colors color, RenderPosition renderPosition, DWORD horizontalAlignment)
{
    Point position = renderPosition.GetPosition();

    // Copy text to prevent preliminary dispose by calling code.
    wchar_t* copiedText = StringUtilities::CopyWideChar(text);

    auto printTextAction = [=](HDC deviceContext) -> void {
        AssertCriticalWinApiResult(SelectObject(deviceContext, font->GetHandle()));
        ExceptionHelper::ThrowOnWinapiError(SetTextColor(deviceContext, (COLORREF)color), true, CLR_INVALID);
        ExceptionHelper::ThrowOnWinapiError(SetTextAlign(deviceContext, TA_BASELINE | horizontalAlignment), true, GDI_ERROR);
        AssertCriticalWinApiResult(SetBkMode(deviceContext, TRANSPARENT));

        AssertCriticalWinApiResult(TextOut(deviceContext, position.GetX(), position.GetY(), copiedText, wcslen(copiedText)));

        delete copiedText;
    };
    renderActions.push_back(printTextAction);

    Size textSize = renderingProvider->GetTextSize(text.c_str(), font);

    int rightX = position.GetX() + textSize.GetWidth();
    int bottomY = position.GetY() - font->GetAscent() + textSize.GetHeight();

    renderedSize = Size(
        max(renderedSize.GetWidth(), rightX),
        max(renderedSize.GetHeight(), bottomY)
    );

    return TextRenderResult(textSize, rightX, position.GetY(), bottomY);
}

void Renderer::DrawRect(Rect rect, Brush* brush)
{
    RECT gdiRect;
    gdiRect.left = rect.GetX();
    gdiRect.right = rect.GetRight();
    gdiRect.top = rect.GetY();
    gdiRect.bottom = rect.GetBottom();

    auto drawRectAction = [=](HDC hdc) -> void {
        AssertCriticalWinApiResult(FillRect(hdc, &gdiRect, brush->GetHandle()));
    };
    renderActions.push_back(drawRectAction);

    UpdateRenderedContentSize(rect);
}

void Renderer::DrawBorderedRect(Rect outerRect, Brush* brush, int borderWidth, Colors borderColor)
{
    int borderAjustments = borderWidth / 2;
    outerRect = Rect(
        outerRect.GetX() + borderAjustments,
        outerRect.GetY() + borderAjustments,
        outerRect.GetWidth() - borderAjustments,
        outerRect.GetHeight() - borderAjustments
    );

    Pen* borderPen = renderingProvider->CreateCustomPen(borderColor, borderWidth);
    HBRUSH contentBrush = brush != nullptr
        ? brush->GetHandle()
        : (HBRUSH)GetStockObject(HOLLOW_BRUSH);

    auto drawRectAction = [=](HDC hdc) -> void {
        AssertCriticalWinApiResult(SelectObject(hdc, borderPen->GetHandle()));
        AssertCriticalWinApiResult(SelectObject(hdc, contentBrush));

        AssertCriticalWinApiResult(Rectangle(hdc, outerRect.GetX(), outerRect.GetY(), outerRect.GetRight(), outerRect.GetBottom()));

        delete borderPen;
    };
    renderActions.push_back(drawRectAction);

    UpdateRenderedContentSize(outerRect);
}

void Renderer::DrawDeviceContext(HDC deviceContext, Size contextSize)
{
    auto drawDeviceContextAction = [=](HDC hdc) -> void {
        deviceContextProvider->CopyDeviceContext(deviceContext, hdc, contextSize);
    };
    renderActions.push_back(drawDeviceContextAction);

    UpdateRenderedContentSize(contextSize);
}

void Renderer::SetBackground(Brush* backgroundBrush)
{
    this->backgroundBrush = backgroundBrush;
}

const Brush* Renderer::GetBackgroundBrush() const
{
    return backgroundBrush == nullptr
        ? defaultBackgroundBrush
        : backgroundBrush;
}

void Renderer::ClearDeviceContext(HDC deviceContext, Size deviceContextSize) const
{
    RECT rect;
    rect.top = 0;
    rect.left = 0;
    rect.bottom = deviceContextSize.GetHeight();
    rect.right = deviceContextSize.GetWidth();

    AssertCriticalWinApiResult(FillRect(deviceContext, &rect, GetBackgroundBrush()->GetHandle()));
}

Size Renderer::GetSize() const
{
    return renderedSize;
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
    renderedSize = Size(
        renderedSize.GetWidth() + widthToAdd,
        renderedSize.GetHeight());
}

void Renderer::IncreaseHeight(int heightToAdd)
{
    renderedSize = Size(
        renderedSize.GetWidth(),
        renderedSize.GetHeight() + heightToAdd);
}

void Renderer::UpdateRenderedContentSize(View* window)
{
    UpdateRenderedContentSize(window->GetBoundingRect());
}

void Renderer::UpdateRenderedContentSize(Rect rect)
{
    renderedSize = Size(
        max(renderedSize.GetWidth(), rect.GetRight()),
        max(renderedSize.GetHeight(), rect.GetBottom())
    );
}

void Renderer::UpdateRenderedContentSize(Size size)
{
    renderedSize = Size(
        max(renderedSize.GetWidth(), size.GetWidth()),
        max(renderedSize.GetHeight(), size.GetHeight())
    );
}

Renderer::~Renderer()
{
    renderActions.clear();
    delete defaultBackgroundBrush;
}
