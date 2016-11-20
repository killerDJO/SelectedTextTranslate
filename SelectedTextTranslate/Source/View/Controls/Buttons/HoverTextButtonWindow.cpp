#include "View\Controls\Buttons\HoverTextButtonWindow.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"

HoverTextButtonWindow::HoverTextButtonWindow(WindowContext* context, Window* parentWindow)
    : HoverButtonWindow(context, parentWindow)
{
    this->font = context->GetRenderingContext()->CreateCustomFont(FontSizes::Normal);
    this->normalColor = Colors::Gray;
    this->hoverColor = Colors::Black;
    this->text = wstring();
}

void HoverTextButtonWindow::SetDescriptor(WindowDescriptor descriptor)
{
    throw new SelectedTextTranslateFatalException(L"SetDescriptor is unsupported");
}

void HoverTextButtonWindow::SetPosition(Point position)
{
    AssertWindowNotInitialized();
    descriptor = WindowDescriptor::CreateStretchWindowDescriptor(position);
}

void HoverTextButtonWindow::SetNormalColor(Colors normalColor)
{
    AssertWindowNotInitialized();
    this->normalColor = normalColor;
}

Colors HoverTextButtonWindow::GetNormalColor() const
{
    return normalColor;
}

void HoverTextButtonWindow::SetHoverColor(Colors hoverColor)
{
    AssertWindowNotInitialized();
    this->hoverColor = hoverColor;
}

Colors HoverTextButtonWindow::GetHoverColor() const
{
    return hoverColor;
}

void HoverTextButtonWindow::SetFont(HFONT font)
{
    AssertWindowNotInitialized();
    this->font = font;
}

HFONT HoverTextButtonWindow::GetFont() const
{
    return font;
}

void HoverTextButtonWindow::SetText(wstring text)
{
    AssertWindowNotInitialized();
    this->text = text;
}

wstring HoverTextButtonWindow::GetText() const
{
    return text;
}

void HoverTextButtonWindow::RenderStatesDeviceContext()
{
    Size textSize = context->GetRenderingContext()->GetTextSize(text.c_str(), font);

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