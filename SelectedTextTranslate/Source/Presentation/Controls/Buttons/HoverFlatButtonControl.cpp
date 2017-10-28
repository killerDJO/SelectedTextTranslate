#include "Presentation\Controls\Buttons\HoverFlatButtonControl.h"

HoverFlatButtonControl::HoverFlatButtonControl(ViewContext* context, View* parentWindow)
    : HoverButtonControl(context, parentWindow)
{
    this->defaultFont = RenderingProvider->CreateCustomFont(FontSizes::Normal);
    this->font = nullptr;
    this->text = wstring();
    this->paddingX = ScaleProvider->Scale(10);
    this->paddingY = ScaleProvider->Scale(5);
    this->borderWidth = ScaleProvider->Scale(1);
    this->ClassName = L"STT_HOVERFLATBUTTON";
    this->Layout = ::LayoutDescriptor();
}

void HoverFlatButtonControl::SetPosition(Point position)
{
    AssertViewNotInitialized();
    Layout.SetPosition(position);
}

void HoverFlatButtonControl::SetFont(Font* font)
{
    AssertViewNotInitialized();
    this->font = font;
}

Font* HoverFlatButtonControl::GetFont() const
{
    return font == nullptr ? defaultFont : font;
}

void HoverFlatButtonControl::SetText(wstring text)
{
    AssertViewNotInitialized();
    this->text = text;
}

wstring HoverFlatButtonControl::GetText() const
{
    return text;
}

void HoverFlatButtonControl::SetPaddingX(int paddingX)
{
    AssertViewNotInitialized();
    this->paddingX = paddingX;
}

int HoverFlatButtonControl::GetPaddingX() const
{
    return paddingX;
}

void HoverFlatButtonControl::SetPaddingY(int paddingY)
{
    AssertViewNotInitialized();
    this->paddingY = paddingY;
}

int HoverFlatButtonControl::GetPaddingY() const
{
    return paddingY;
}

Size HoverFlatButtonControl::GetComputedSize() const
{
    Size textSize = RenderingProvider->GetTextSize(text, GetFont());

    return Size(
        textSize.GetWidth() + paddingX * 2,
        GetFont()->GetStrokeHeight() + 1 + (paddingY + borderWidth) * 2);
}

void HoverFlatButtonControl::Initialize()
{
    Layout = LayoutDescriptor::CreateLayoutDescriptor(
        Layout.GetPosition(),
        GetComputedSize(),
        OverflowModes::Fixed,
        OverflowModes::Fixed);

    HoverButtonControl::Initialize();
}

int HoverFlatButtonControl::GetTextBaseline() const
{
    return GetFont()->GetStrokeHeight() + paddingY + borderWidth;
}

void HoverFlatButtonControl::RenderStatesDeviceContexts()
{
    stateToDeviceContextMap[ButtonStates::Normal] = DeviceContextProvider->CreateDeviceContext(GetSize());
    stateToDeviceContextMap[ButtonStates::Hovered] = DeviceContextProvider->CreateDeviceContext(GetSize());
    stateToDeviceContextMap[ButtonStates::Pressed] = DeviceContextProvider->CreateDeviceContext(GetSize());
    stateToDeviceContextMap[ButtonStates::Disabled] = DeviceContextProvider->CreateDeviceContext(GetSize());

    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Normal], Colors::Button, Colors::LightGray, Colors::Black);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Hovered], Colors::ButtonHovered, Colors::Blue, Colors::Black);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Pressed], Colors::ButtonPressed, Colors::Blue, Colors::Black);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Disabled], Colors::Background, Colors::LightGray, Colors::LightGray);
}

void HoverFlatButtonControl::RenderStateDeviceContext(HDC deviceContext, Colors backgroundColor, Colors borderColor, Colors fontColor) const
{
    Renderer* renderer = RenderingContext->GetRenderer();

    Brush* backgroundBrush = RenderingProvider->CreateCustomBrush(backgroundColor);

    renderer->DrawBorderedRect(
        Rect(Point(0, 0), GetSize()),
        backgroundBrush,
        borderWidth,
        borderColor);

    renderer->PrintText(
        text.c_str(),
        GetFont(),
        fontColor,
        Point(GetSize().GetWidth() / 2, GetTextBaseline()),
        TA_CENTER);

    renderer->Render(deviceContext, GetSize());

    RenderingContext->ReleaseRenderer(renderer);

    delete backgroundBrush;
}

HoverFlatButtonControl::~HoverFlatButtonControl()
{
    delete defaultFont;
}