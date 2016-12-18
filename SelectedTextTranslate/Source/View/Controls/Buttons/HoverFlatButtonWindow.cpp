#include "View\Controls\Buttons\HoverFlatButtonWindow.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"

HoverFlatButtonWindow::HoverFlatButtonWindow(WindowContext* context, Window* parentWindow)
    : HoverButtonWindow(context, parentWindow)
{
    this->defaultFont = context->GetRenderingContext()->CreateCustomFont(FontSizes::Normal);
    this->font = nullptr;
    this->text = wstring();
    this->paddingX = 10;
    this->paddingY = 5;
    this->borderWidth = 1;
    this->className = L"STT_HOVERFLATBUTTON";
}

void HoverFlatButtonWindow::SetDescriptor(WindowDescriptor descriptor)
{
    throw new SelectedTextTranslateFatalException(L"SetDescriptor is unsupported");
}

void HoverFlatButtonWindow::SetPosition(PointReal position)
{
    AssertWindowNotInitialized();
    this->position = context->GetScaleProvider()->Scale(position);
}

void HoverFlatButtonWindow::SetFont(HFONT font)
{
    AssertWindowNotInitialized();
    this->font = font;
}

HFONT HoverFlatButtonWindow::GetFont() const
{
    return font == nullptr ? defaultFont : font;
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

void HoverFlatButtonWindow::SetPaddingX(int paddingX)
{
    AssertWindowNotInitialized();
    this->paddingX = paddingX;
}

int HoverFlatButtonWindow::GetPaddingX() const
{
    return paddingX;
}

void HoverFlatButtonWindow::SetPaddingY(int paddingY)
{
    AssertWindowNotInitialized();
    this->paddingY = paddingY;
}

int HoverFlatButtonWindow::GetPaddingY() const
{
    return paddingY;
}

void HoverFlatButtonWindow::Initialize()
{
    int fontStrokeHeight = context->GetRenderingContext()->GetFontStrokeHeight(GetFont());
    Size textSize = context->GetRenderingContext()->GetTextSize(text, GetFont());
    descriptor = WindowDescriptor::CreateWindowDescriptor(
        position,
        Size(
            textSize.GetWidth() + context->GetScaleProvider()->Scale(paddingX) * 2,
            fontStrokeHeight + context->GetScaleProvider()->Scale(paddingY + borderWidth) * 2),
        OverflowModes::Fixed,
        OverflowModes::Fixed);

    HoverButtonWindow::Initialize();
}

double HoverFlatButtonWindow::GetTextBaseline() const
{
    double fontStrokeHeight = context->GetScaleProvider()->Downscale(context->GetRenderingContext()->GetFontStrokeHeight(GetFont()));
    return fontStrokeHeight + paddingY + borderWidth;
}

void HoverFlatButtonWindow::RenderStatesDeviceContext()
{
    stateToDeviceContextMap[ButtonStates::Normal] = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);
    stateToDeviceContextMap[ButtonStates::Hovered] = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);
    stateToDeviceContextMap[ButtonStates::Pressed] = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);
    stateToDeviceContextMap[ButtonStates::Disabled] = context->GetDeviceContextProvider()->CreateDeviceContext(windowSize);

    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Normal], Colors::Button, Colors::LightGray, Colors::Black);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Hovered], Colors::ButtonHovered, Colors::Blue, Colors::Black);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Pressed], Colors::ButtonPressed, Colors::Blue, Colors::Black);
    RenderStateDeviceContext(stateToDeviceContextMap[ButtonStates::Disabled], Colors::Background, Colors::LightGray, Colors::LightGray);
}

void HoverFlatButtonWindow::RenderStateDeviceContext(HDC deviceContext, Colors backgroundColor, Colors borderColor, Colors fontColor) const
{
    Renderer* renderer = context->GetRenderingContext()->GetRenderer();

    HBRUSH backgroundBrush = context->GetRenderingContext()->CreateCustomBrush(backgroundColor);

    renderer->DrawBorderedRect(
        RectReal(PointReal(0, 0), GetDownscaledSize()),
        backgroundBrush,
        borderWidth,
        borderColor);

    renderer->PrintText(text.c_str(), GetFont(), fontColor, PointReal(GetDownscaledSize().GetWidth() / 2, GetTextBaseline()), TA_CENTER);

    renderer->Render(deviceContext, GetScaledSize());

    context->GetRenderingContext()->ReleaseRenderer(renderer);

    context->GetRenderingContext()->DeleteCustomBrush(backgroundBrush);
}

HoverFlatButtonWindow::~HoverFlatButtonWindow()
{
    context->GetRenderingContext()->DeleteCustomFont(defaultFont);
}