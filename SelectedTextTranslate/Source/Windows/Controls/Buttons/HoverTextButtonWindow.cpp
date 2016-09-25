#include "Windows\Controls\Buttons\HoverTextButtonWindow.h"

HoverTextButtonWindow::HoverTextButtonWindow(WindowContext* context, WindowDescriptor descriptor, HWND parentWindow, HFONT font, Colors normalColor, Colors hoverColor, wstring text, function<void()> clickCallback)
    : HoverButtonWindow(context, descriptor, parentWindow, clickCallback)
{
    this->font = font;
    this->normalColor = normalColor;
    this->hoverColor = hoverColor;
    this->text = text;
}

void HoverTextButtonWindow::RenderStatesDC()
{
    Size textSize = context->GetRenderingContext()->GetTextSize(inMemoryDC, text.c_str(), font);

    windowSize.Width = max(windowSize.Width, textSize.Width);
    windowSize.Height = max(windowSize.Height, textSize.Height);

    normalStateDC = context->GetDeviceContextProvider()->CreateInMemoryDC(windowSize);
    hoverStateDC = context->GetDeviceContextProvider()->CreateInMemoryDC(windowSize);
    context->GetDeviceContextProvider()->ResizeDC(inMemoryDC, windowSize);

    RenderStateDC(normalStateDC, normalColor);
    RenderStateDC(hoverStateDC, hoverColor);
}

void HoverTextButtonWindow::RenderStateDC(HDC hdc, Colors color) const
{
    Renderer* renderer = context->GetRenderingContext()->GetRenderer(hdc);

    renderer->ClearDC(hdc, windowSize);
    int fontAscent = renderer->GetFontAscent(font);
    renderer->PrintText(text.c_str(), font, color, Point(0, fontAscent));

    context->GetRenderingContext()->ReleaseRenderer(renderer);
}

HoverTextButtonWindow::~HoverTextButtonWindow()
{
}