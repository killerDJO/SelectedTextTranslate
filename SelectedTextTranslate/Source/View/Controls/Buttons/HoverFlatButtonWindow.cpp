#include "View\Controls\Buttons\HoverFlatButtonWindow.h"

HoverFlatButtonWindow::HoverFlatButtonWindow(WindowContext* context, WindowDescriptor descriptor, wstring name, Window* parentWindow, HFONT font, wstring text, int padding)
    : HoverButtonWindow(context, descriptor, name, parentWindow)
{
    this->font = font;
    this->text = text;
    this->padding = context->GetScaleProvider()->Scale(padding);
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
    renderer->PrintText(text.c_str(), font, Colors::Black, Point(context->GetScaleProvider()->Downscale(windowSize.Width / 2), padding + fontAscent), TA_CENTER);

    renderer->Render(deviceContext, windowSize);

    context->GetRenderingContext()->ReleaseRenderer(renderer);
}

HoverFlatButtonWindow::~HoverFlatButtonWindow()
{
}