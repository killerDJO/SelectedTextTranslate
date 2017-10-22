#include "Presentation\Controls\Buttons\HoverTextButtonWindow.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"

HoverTextButtonWindow::HoverTextButtonWindow(CommonContext* context, View* parentWindow)
    : HoverButtonWindow(context, parentWindow)
{
    this->defaultFont = renderingProvider->CreateCustomFont(FontSizes::Normal);
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
    AssertViewNotInitialized();
    // Important to give window initial size. Otherwise it will not be initially showed in layered mode.
    this->descriptor = WindowDescriptor::CreateWindowDescriptor(
        position,
        Size(1, 1),
        OverflowModes::Stretch,
        OverflowModes::Stretch);
}

void HoverTextButtonWindow::SetNormalColor(Colors normalColor)
{
    AssertViewNotInitialized();
    this->normalColor = normalColor;
}

Colors HoverTextButtonWindow::GetNormalColor() const
{
    return normalColor;
}

void HoverTextButtonWindow::SetHoverColor(Colors hoverColor)
{
    AssertViewNotInitialized();
    this->hoverColor = hoverColor;
}

Colors HoverTextButtonWindow::GetHoverColor() const
{
    return hoverColor;
}

void HoverTextButtonWindow::SetDisabledColor(Colors disabledColor)
{
    AssertViewNotInitialized();
    this->disabledColor = disabledColor;
}

Colors HoverTextButtonWindow::GetDisabledColor() const
{
    return disabledColor;
}

void HoverTextButtonWindow::SetBackgroundColor(Colors backgroundColor)
{
    AssertViewNotInitialized();
    this->backgroundColor = backgroundColor;
}

Colors HoverTextButtonWindow::GetBackgroundColor() const
{
    return backgroundColor;
}

void HoverTextButtonWindow::SetFont(Font* font)
{
    AssertViewNotInitialized();
    this->font = font;
}

Font* HoverTextButtonWindow::GetFont() const
{
    return font == nullptr ? defaultFont : font;
}

void HoverTextButtonWindow::SetText(wstring text)
{
    AssertViewNotInitialized();
    this->text = text;
}

wstring HoverTextButtonWindow::GetText() const
{
    return text;
}

void HoverTextButtonWindow::RenderStatesDeviceContexts()
{
    Size textSize = renderingProvider->GetTextSize(text.c_str(), GetFont());

    nativeStateDescriptor.EnsureSize(textSize);

    stateToDeviceContextMap[ButtonStates::Normal] = deviceContextProvider->CreateDeviceContext(nativeStateDescriptor.GetSize());
    stateToDeviceContextMap[ButtonStates::Hovered] = deviceContextProvider->CreateDeviceContext(nativeStateDescriptor.GetSize());
    stateToDeviceContextMap[ButtonStates::Disabled] = deviceContextProvider->CreateDeviceContext(nativeStateDescriptor.GetSize());
    stateToDeviceContextMap[ButtonStates::Pressed] = stateToDeviceContextMap[ButtonStates::Hovered];
    deviceContextBuffer->Resize(nativeStateDescriptor.GetSize());

    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Normal], normalColor);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Hovered], hoverColor);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Disabled], disabledColor);
}

void HoverTextButtonWindow::RenderStateDeviceContext(HDC deviceContext, Colors color) const
{
    Renderer* renderer = renderingContext->GetRenderer();

    Brush* backgroundBrush = renderingProvider->CreateCustomBrush(backgroundColor);
    renderer->SetBackground(backgroundBrush);

    renderer->PrintText(text.c_str(), GetFont(), color, Point(0, GetFont()->GetAscent()));
    renderer->Render(deviceContext, deviceContextBuffer->GetSize());

    renderingContext->ReleaseRenderer(renderer);

    delete backgroundBrush;
}

HoverTextButtonWindow::~HoverTextButtonWindow()
{
    delete defaultFont;
}