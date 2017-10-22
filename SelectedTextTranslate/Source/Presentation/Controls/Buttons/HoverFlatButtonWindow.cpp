#include "Presentation\Controls\Buttons\HoverFlatButtonWindow.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"

HoverFlatButtonWindow::HoverFlatButtonWindow(CommonContext* context, View* parentWindow)
    : HoverButtonWindow(context, parentWindow)
{
    this->defaultFont = renderingProvider->CreateCustomFont(FontSizes::Normal);
    this->font = nullptr;
    this->text = wstring();
    this->paddingX = scaleProvider->Scale(10);
    this->paddingY = scaleProvider->Scale(5);
    this->borderWidth = scaleProvider->Scale(1);
    this->className = L"STT_HOVERFLATBUTTON";
    this->descriptor = WindowDescriptor();
}

void HoverFlatButtonWindow::SetDescriptor(WindowDescriptor descriptor)
{
    throw new SelectedTextTranslateFatalException(L"SetDescriptor is unsupported");
}

void HoverFlatButtonWindow::SetPosition(Point position)
{
    AssertViewNotInitialized();
    descriptor.SetPosition(position);
}

void HoverFlatButtonWindow::SetFont(Font* font)
{
    AssertViewNotInitialized();
    this->font = font;
}

Font* HoverFlatButtonWindow::GetFont() const
{
    return font == nullptr ? defaultFont : font;
}

void HoverFlatButtonWindow::SetText(wstring text)
{
    AssertViewNotInitialized();
    this->text = text;
}

wstring HoverFlatButtonWindow::GetText() const
{
    return text;
}

void HoverFlatButtonWindow::SetPaddingX(int paddingX)
{
    AssertViewNotInitialized();
    this->paddingX = paddingX;
}

int HoverFlatButtonWindow::GetPaddingX() const
{
    return paddingX;
}

void HoverFlatButtonWindow::SetPaddingY(int paddingY)
{
    AssertViewNotInitialized();
    this->paddingY = paddingY;
}

int HoverFlatButtonWindow::GetPaddingY() const
{
    return paddingY;
}

Size HoverFlatButtonWindow::GetComputedSize() const
{
    Size textSize = renderingProvider->GetTextSize(text, GetFont());

    return Size(
        textSize.GetWidth() + paddingX * 2,
        GetFont()->GetStrokeHeight() + 1 + (paddingY + borderWidth) * 2);
}

void HoverFlatButtonWindow::Initialize()
{
    descriptor = WindowDescriptor::CreateWindowDescriptor(
        descriptor.GetPosition(),
        GetComputedSize(),
        OverflowModes::Fixed,
        OverflowModes::Fixed);

    HoverButtonWindow::Initialize();
}

int HoverFlatButtonWindow::GetTextBaseline() const
{
    return GetFont()->GetStrokeHeight() + paddingY + borderWidth;
}

void HoverFlatButtonWindow::RenderStatesDeviceContexts()
{
    stateToDeviceContextMap[ButtonStates::Normal] = deviceContextProvider->CreateDeviceContext(GetSize());
    stateToDeviceContextMap[ButtonStates::Hovered] = deviceContextProvider->CreateDeviceContext(GetSize());
    stateToDeviceContextMap[ButtonStates::Pressed] = deviceContextProvider->CreateDeviceContext(GetSize());
    stateToDeviceContextMap[ButtonStates::Disabled] = deviceContextProvider->CreateDeviceContext(GetSize());

    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Normal], Colors::Button, Colors::LightGray, Colors::Black);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Hovered], Colors::ButtonHovered, Colors::Blue, Colors::Black);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Pressed], Colors::ButtonPressed, Colors::Blue, Colors::Black);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Disabled], Colors::Background, Colors::LightGray, Colors::LightGray);
}

void HoverFlatButtonWindow::RenderStateDeviceContext(HDC deviceContext, Colors backgroundColor, Colors borderColor, Colors fontColor) const
{
    Renderer* renderer = renderingContext->GetRenderer();

    Brush* backgroundBrush = renderingProvider->CreateCustomBrush(backgroundColor);

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

    renderingContext->ReleaseRenderer(renderer);

    delete backgroundBrush;
}

HoverFlatButtonWindow::~HoverFlatButtonWindow()
{
    delete defaultFont;
}