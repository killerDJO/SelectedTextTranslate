#include "View\Controls\Buttons\HoverFlatButtonWindow.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"

HoverFlatButtonWindow::HoverFlatButtonWindow(WindowContext* context, Window* parentWindow)
    : HoverButtonWindow(context, parentWindow)
{
    this->font = context->GetRenderingContext()->CreateCustomFont(FontSizes::Normal);
    this->text = wstring();
    this->padding = 2;
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

void HoverFlatButtonWindow::RenderStatesDeviceContext()
{
    stateToDeviceContextMap[ButtonStates::Normal] = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);
    stateToDeviceContextMap[ButtonStates::Hovered] = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);
    stateToDeviceContextMap[ButtonStates::Pressed] = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);

    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Normal], Colors::Button, Colors::LightGray);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Hovered], Colors::ButtonHovered, Colors::Blue);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Pressed], Colors::ButtonPressed, Colors::Blue);
}

void HoverFlatButtonWindow::RenderStateDeviceContext(HDC deviceContext, Colors backgroundColor, Colors borderColor) const
{
    Renderer* renderer = context->GetRenderingContext()->GetRenderer();

    Rect borderRect = Rect(
        Point(0, 0),
        context->GetScaleProvider()->Downscale(GetAvailableClientSize()));

    renderer->DrawBorderedRect(
        borderRect,
        context->GetRenderingContext()->CreateCustomBrush(backgroundColor),
        1,
        borderColor);

    int fontAscent = renderer->GetFontAscent(font);
    int scaledWindowWidth = context->GetScaleProvider()->Downscale(windowSize.Width);
    renderer->PrintText(text.c_str(), font, Colors::Black, Point(scaledWindowWidth / 2, padding + fontAscent), TA_CENTER);

    renderer->Render(deviceContext, windowSize);

    context->GetRenderingContext()->ReleaseRenderer(renderer);
}

HoverFlatButtonWindow::~HoverFlatButtonWindow()
{
}