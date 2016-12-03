#include "View\Controls\Buttons\HoverTextButtonWindow.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"

HoverTextButtonWindow::HoverTextButtonWindow(WindowContext* context, Window* parentWindow)
    : HoverButtonWindow(context, parentWindow)
{
    this->defaultFont = context->GetRenderingContext()->CreateCustomFont(FontSizes::Normal);
    this->font = nullptr;
    this->normalColor = Colors::Gray;
    this->hoverColor = Colors::Black;
    this->disabledColor = Colors::LightGray;
    this->backgroundColor = Colors::White;
    this->text = wstring();
    this->className = L"STT_HOVERTEXTBUTTON";
}

void HoverTextButtonWindow::SetDescriptor(WindowDescriptor descriptor)
{
    throw new SelectedTextTranslateFatalException(L"SetDescriptor is unsupported");
}

void HoverTextButtonWindow::SetPosition(Point position)
{
    AssertWindowNotInitialized();
    this->position = position;
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

void HoverTextButtonWindow::SetDisabledColor(Colors disabledColor)
{
    AssertWindowNotInitialized();
    this->disabledColor = disabledColor;
}

Colors HoverTextButtonWindow::GetDisabledColor() const
{
    return disabledColor;
}

void HoverTextButtonWindow::SetBackgroundColor(Colors backgroundColor)
{
    AssertWindowNotInitialized();
    this->backgroundColor = backgroundColor;
}

Colors HoverTextButtonWindow::GetBackgroundColor() const
{
    return backgroundColor;
}

void HoverTextButtonWindow::SetFont(HFONT font)
{
    AssertWindowNotInitialized();
    this->font = font;
}

HFONT HoverTextButtonWindow::GetFont() const
{
    return font == nullptr ? defaultFont : font;
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

void HoverTextButtonWindow::Initialize()
{
    // Important to give window initial size. Otherwise it will not be initially showed in layered mode.
    descriptor = WindowDescriptor::CreateWindowDescriptor(position, Size(1, 1), OverflowModes::Stretch, OverflowModes::Stretch);
    HoverButtonWindow::Initialize();
}

void HoverTextButtonWindow::RenderStatesDeviceContext()
{
    Size textSize = context->GetRenderingContext()->GetTextSize(text.c_str(), GetFont());

    windowSize.Width = max(windowSize.Width, textSize.Width);
    windowSize.Height = max(windowSize.Height, textSize.Height);

    stateToDeviceContextMap[ButtonStates::Normal] = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);
    stateToDeviceContextMap[ButtonStates::Hovered] = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);
    stateToDeviceContextMap[ButtonStates::Disabled] = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);
    stateToDeviceContextMap[ButtonStates::Pressed] = stateToDeviceContextMap[ButtonStates::Hovered];
    deviceContextBuffer->Resize(windowSize);

    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Normal], normalColor);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Hovered], hoverColor);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Disabled], disabledColor);
}

void HoverTextButtonWindow::RenderStateDeviceContext(HDC deviceContext, Colors color) const
{
    Renderer* renderer = context->GetRenderingContext()->GetRenderer();

    HBRUSH backgroudBrush = context->GetRenderingContext()->CreateCustomBrush(backgroundColor);
    renderer->SetBackground(backgroudBrush);

    int fontAscent = renderer->GetFontAscent(GetFont());
    renderer->PrintText(text.c_str(), GetFont(), color, Point(0, fontAscent));
    renderer->Render(deviceContext, deviceContextBuffer->GetSize());

    context->GetRenderingContext()->ReleaseRenderer(renderer);

    context->GetRenderingContext()->DeleteCustomBrush(backgroudBrush);
}

HoverTextButtonWindow::~HoverTextButtonWindow()
{
    context->GetRenderingContext()->DeleteCustomFont(defaultFont);
}