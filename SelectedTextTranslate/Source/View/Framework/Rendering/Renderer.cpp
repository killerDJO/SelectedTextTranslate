#include "View\Framework\Rendering\Renderer.h"
#include "Utilities\StringUtilities.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "View\Framework\Rendering\Objects\Brush.h"
#include "View\Framework\Rendering\Objects\Font.h"

Renderer::Renderer(RenderingContext* renderingContext)
{
    this->renderingContext = renderingContext;
    this->originalSize = Size(0, 0);
    this->backgroundBrush = renderingContext->CreateCustomBrush(Colors::White);
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

    Size textSize = renderingContext->GetTextSize(text.c_str(), font);

    originalSize = Size(
        max(originalSize.GetWidth(), position.GetX() + textSize.GetWidth()),
        max(originalSize.GetHeight(), position.GetY() + textSize.GetHeight() - font->GetAscent())
    );

    int rightX = position.GetX() + textSize.GetWidth();
    int bottomY = position.GetY() - font->GetAscent() + textSize.GetHeight();

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

    originalSize = Size(
        max(originalSize.GetWidth(), rect.GetWidth()),
        max(originalSize.GetHeight(), rect.GetHeight())
    );
}

void Renderer::DrawBorderedRect(Rect rect, Brush* brush, int borderWidth, Colors borderColor)
{
    int borderAjustments = borderWidth / 2;
    rect = Rect(
        rect.GetX() + borderAjustments,
        rect.GetY() + borderAjustments,
        rect.GetWidth() - borderAjustments,
        rect.GetHeight() - borderAjustments
    );

    Pen* borderPen = renderingContext->CreateCustomPen(borderColor, borderWidth);
    HBRUSH contentBrush = brush != nullptr
        ? brush->GetHandle()
        : (HBRUSH)GetStockObject(HOLLOW_BRUSH);

    auto drawRectAction = [=](HDC hdc) -> void {
        AssertCriticalWinApiResult(SelectObject(hdc, borderPen->GetHandle()));
        AssertCriticalWinApiResult(SelectObject(hdc, contentBrush));

        AssertCriticalWinApiResult(Rectangle(hdc, rect.GetX(), rect.GetY(), rect.GetRight(), rect.GetBottom()));

        delete borderPen;
    };
    renderActions.push_back(drawRectAction);

    originalSize = Size(
        max(originalSize.GetWidth(), rect.GetWidth()),
        max(originalSize.GetHeight(), rect.GetHeight())
    );
}

void Renderer::SetBackground(Brush* backgroundBrush)
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

    AssertCriticalWinApiResult(FillRect(deviceContext, &rect, backgroundBrush->GetHandle()));
}

Size Renderer::GetSize() const
{
    return originalSize;
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
    originalSize = Size(
        originalSize.GetWidth() + widthToAdd,
        originalSize.GetHeight());
}

void Renderer::IncreaseHeight(int heightToAdd)
{
    originalSize = Size(
        originalSize.GetWidth(),
        originalSize.GetHeight() + heightToAdd);
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
