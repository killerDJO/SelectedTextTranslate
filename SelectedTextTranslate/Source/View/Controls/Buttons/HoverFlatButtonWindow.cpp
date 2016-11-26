#include "View\Controls\Buttons\HoverFlatButtonWindow.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

HoverFlatButtonWindow::HoverFlatButtonWindow(WindowContext* context, Window* parentWindow)
    : HoverButtonWindow(context, parentWindow)
{
    this->font = context->GetRenderingContext()->CreateCustomFont(FontSizes::Normal);
    this->text = wstring();
    this->padding = 3;
    this->className = L"STT_HOVERFLATBUTTON";
}

void HoverFlatButtonWindow::SetDescriptor(WindowDescriptor descriptor)
{
    throw new SelectedTextTranslateFatalException(L"SetDescriptor is unsupported");
}

void HoverFlatButtonWindow::SetDimensions(Point position, Size size)
{
    AssertWindowNotInitialized();
    descriptor = WindowDescriptor::CreateFixedWindowDescriptor(position, size);
}

void HoverFlatButtonWindow::SetFont(HFONT font)
{
    AssertWindowNotInitialized();
    this->font = font;
}

HFONT HoverFlatButtonWindow::GetFont() const
{
    return font;
}

void HoverFlatButtonWindow::SetText(wstring text)
{
    AssertWindowNotInitialized();
    this->text = text;
}

wstring HoverFlatButtonWindow::GetText() const
{
    return text;
}

void HoverFlatButtonWindow::SetPadding(int padding)
{
    AssertWindowNotInitialized();
    this->padding = padding;
}

int HoverFlatButtonWindow::GetPadding() const
{
    return padding;
}

int HoverFlatButtonWindow::GetTextBaseline() const
{
    int fontDescent = context->GetScaleProvider()->Downscale(context->GetRenderingContext()->GetFontMetrics(font).tmDescent);
    return GetSize(true).Height - padding - fontDescent;
}

void HoverFlatButtonWindow::RenderStatesDeviceContext()
{
    stateToDeviceContextMap[ButtonStates::Normal] = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);
    stateToDeviceContextMap[ButtonStates::Hovered] = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);
    stateToDeviceContextMap[ButtonStates::Pressed] = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);
    stateToDeviceContextMap[ButtonStates::Disabled] = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);

    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Normal], Colors::Button, Colors::LightGray, Colors::Black);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Hovered], Colors::ButtonHovered, Colors::Blue, Colors::Black);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Pressed], Colors::ButtonPressed, Colors::Blue, Colors::Black);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Disabled], Colors::Background, Colors::LightGray, Colors::LightGray);
}

void HoverFlatButtonWindow::RenderStateDeviceContext(HDC deviceContext, Colors backgroundColor, Colors borderColor, Colors fontColor) const
{
    Renderer* renderer = context->GetRenderingContext()->GetRenderer();

    HBRUSH backgroundBrush = context->GetRenderingContext()->CreateCustomBrush(backgroundColor);
    Size windowSize = GetSize(true);

    renderer->DrawBorderedRect(
        Rect(Point(0, 0), windowSize),
        backgroundBrush,
        1,
        borderColor);

    int fontDescent = renderer->GetFontDescent(font);
    renderer->PrintText(text.c_str(), font, fontColor, Point(windowSize.Width / 2, windowSize.Height - padding - fontDescent), TA_CENTER);

    renderer->Render(deviceContext, windowSize);

    context->GetRenderingContext()->ReleaseRenderer(renderer);

    AssertCriticalWinApiResult(DeleteObject(backgroundBrush));
}

HoverFlatButtonWindow::~HoverFlatButtonWindow()
{
}
