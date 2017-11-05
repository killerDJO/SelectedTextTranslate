#include "Presentation\Controls\Buttons\HoverTextButtonControl.h"

HoverTextButtonControl::HoverTextButtonControl(ViewContext* context, View* parentWindow)
    : HoverButtonControl(context, parentWindow)
{
    defaultFont = RenderingProvider->CreateCustomFont(FontSizes::Normal);
    font = nullptr;
    normalColor = Colors::Gray;
    hoverColor = Colors::Black;
    disabledColor = Colors::LightGray;
    backgroundColor = Colors::White;
    text = wstring();

    ClassName = L"STT_HOVERTEXTBUTTON";
}

void HoverTextButtonControl::SetPosition(Point position)
{
    AssertViewNotInitialized();
    // Important to give window initial size. Otherwise it will not be initially shown in layered mode.
    ViewState->SetLayout(LayoutDescriptor::CreateLayoutDescriptor(
        position,
        Size(1, 1),
        OverflowModes::Stretch,
        OverflowModes::Stretch));
}

void HoverTextButtonControl::SetNormalColor(Colors normalColor)
{
    AssertViewNotInitialized();
    this->normalColor = normalColor;
}

Colors HoverTextButtonControl::GetNormalColor() const
{
    return normalColor;
}

void HoverTextButtonControl::SetHoverColor(Colors hoverColor)
{
    AssertViewNotInitialized();
    this->hoverColor = hoverColor;
}

Colors HoverTextButtonControl::GetHoverColor() const
{
    return hoverColor;
}

void HoverTextButtonControl::SetDisabledColor(Colors disabledColor)
{
    AssertViewNotInitialized();
    this->disabledColor = disabledColor;
}

Colors HoverTextButtonControl::GetDisabledColor() const
{
    return disabledColor;
}

void HoverTextButtonControl::SetBackgroundColor(Colors backgroundColor)
{
    AssertViewNotInitialized();
    this->backgroundColor = backgroundColor;
}

Colors HoverTextButtonControl::GetBackgroundColor() const
{
    return backgroundColor;
}

void HoverTextButtonControl::SetFont(Font* font)
{
    AssertViewNotInitialized();
    this->font = font;
}

Font* HoverTextButtonControl::GetFont() const
{
    return font == nullptr ? defaultFont : font;
}

void HoverTextButtonControl::SetText(wstring text)
{
    AssertViewNotInitialized();
    this->text = text;
}

wstring HoverTextButtonControl::GetText() const
{
    return text;
}

void HoverTextButtonControl::RenderStatesDeviceContexts()
{
    Size textSize = RenderingProvider->GetTextSize(text.c_str(), GetFont());

    LayoutDescriptor layout = ViewState->GetLayout();
    layout.SetSize(textSize);
    ViewState->SetLayout(layout);

    stateToDeviceContextMap[ButtonStates::Normal] = DeviceContextProvider->CreateDeviceContext(ViewState->GetViewSize());
    stateToDeviceContextMap[ButtonStates::Hovered] = DeviceContextProvider->CreateDeviceContext(ViewState->GetViewSize());
    stateToDeviceContextMap[ButtonStates::Disabled] = DeviceContextProvider->CreateDeviceContext(ViewState->GetViewSize());
    stateToDeviceContextMap[ButtonStates::Pressed] = stateToDeviceContextMap[ButtonStates::Hovered];

    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Normal], normalColor);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Hovered], hoverColor);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Disabled], disabledColor);
}

void HoverTextButtonControl::RenderStateDeviceContext(HDC deviceContext, Colors color) const
{
    Renderer* renderer = RenderingContext->GetRenderer();

    Brush* backgroundBrush = RenderingProvider->CreateCustomBrush(backgroundColor);
    renderer->SetBackground(backgroundBrush);

    renderer->PrintText(text.c_str(), GetFont(), color, Point(0, GetFont()->GetAscent()));
    renderer->Render(deviceContext, ViewState->GetViewSize());

    RenderingContext->ReleaseRenderer(renderer);

    delete backgroundBrush;
}

HoverTextButtonControl::~HoverTextButtonControl()
{
    delete defaultFont;
}