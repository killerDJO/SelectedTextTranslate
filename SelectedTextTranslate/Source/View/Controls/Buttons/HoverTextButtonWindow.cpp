#include "View\Controls\Buttons\HoverTextButtonWindow.h"

HoverTextButtonWindow::HoverTextButtonWindow(WindowContext* context, WindowDescriptor descriptor, wstring name, Window* parentWindow, HFONT font, Colors normalColor, Colors hoverColor, wstring text)
    : HoverButtonWindow(context, descriptor, name, parentWindow)
{
    this->font = font;
    this->normalColor = normalColor;
    this->hoverColor = hoverColor;
    this->text = text;
}

void HoverTextButtonWindow::RenderStatesDeviceContext()
{
    Size textSize = context->GetRenderingContext()->GetTextSize(deviceContextBuffer->GetDeviceContext(), text.c_str(), font);

    windowSize.Width = max(windowSize.Width, textSize.Width);
    windowSize.Height = max(windowSize.Height, textSize.Height);

    stateToDeviceContextMap[ButtonStates::Normal] = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);
    stateToDeviceContextMap[ButtonStates::Hovered] = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);
    stateToDeviceContextMap[ButtonStates::Pressed] = stateToDeviceContextMap[ButtonStates::Hovered];
    deviceContextBuffer->Resize(windowSize);

    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Normal], normalColor);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Hovered], hoverColor);
}

void HoverTextButtonWindow::RenderStateDeviceContext(HDC deviceContext, Colors color) const
{
    Renderer* renderer = context->GetRenderingContext()->GetRenderer();

    int fontAscent = renderer->GetFontAscent(font);
    renderer->PrintText(text.c_str(), font, color, Point(0, fontAscent));
    renderer->Render(deviceContext, deviceContextBuffer->GetSize());

    context->GetRenderingContext()->ReleaseRenderer(renderer);
}

HoverTextButtonWindow::~HoverTextButtonWindow()
{
}