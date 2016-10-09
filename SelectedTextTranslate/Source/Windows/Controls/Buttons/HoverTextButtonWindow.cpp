#include "Windows\Controls\Buttons\HoverTextButtonWindow.h"

HoverTextButtonWindow::HoverTextButtonWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow, HFONT font, Colors normalColor, Colors hoverColor, wstring text, function<void()> clickCallback)
    : HoverButtonWindow(context, descriptor, parentWindow, clickCallback)
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

    normalStateDeviceContext = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);
    hoverStateDeviceContext = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);
    deviceContextBuffer->Resize(windowSize);

    RenderStateDeviceContext(normalStateDeviceContext, normalColor);
    RenderStateDeviceContext(hoverStateDeviceContext, hoverColor);
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