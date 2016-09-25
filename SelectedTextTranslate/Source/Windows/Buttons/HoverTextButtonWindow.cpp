#include "Windows\Buttons\HoverTextButtonWindow.h"

HoverTextButtonWindow::HoverTextButtonWindow(HINSTANCE hInstance, RenderingContext* renderingContext, ScrollProvider* scrollProvider, WindowDescriptor descriptor, HWND parentWindow, HFONT font, Colors normalColor, Colors hoverColor, wstring text, function<void()> clickCallback)
    : HoverButtonWindow(hInstance, renderingContext, scrollProvider, descriptor, parentWindow, clickCallback)
{
    this->font = font;
    this->normalColor = normalColor;
    this->hoverColor = hoverColor;
    this->text = text;
}

void HoverTextButtonWindow::RenderStatesDC()
{
    Size textSize = renderingContext->GetTextSize(inMemoryDC, text.c_str(), font);

    windowSize.Width = max(windowSize.Width, textSize.Width);
    windowSize.Height = max(windowSize.Height, textSize.Height);

    normalStateDC = renderingContext->CreateInMemoryDC(windowSize);
    hoverStateDC = renderingContext->CreateInMemoryDC(windowSize);
    renderingContext->ResizeDC(inMemoryDC, windowSize);

    RenderStateDC(normalStateDC, normalColor);
    RenderStateDC(hoverStateDC, hoverColor);
}

void HoverTextButtonWindow::RenderStateDC(HDC hdc, Colors color) const
{
    Renderer* renderer = new Renderer(hdc, renderingContext);

    renderer->ClearDC(hdc, windowSize);
    int fontAscent = renderer->GetFontAscent(font);
    renderer->PrintText(text.c_str(), font, color, Point(0, fontAscent));

    delete renderer;
}

HoverTextButtonWindow::~HoverTextButtonWindow()
{
}