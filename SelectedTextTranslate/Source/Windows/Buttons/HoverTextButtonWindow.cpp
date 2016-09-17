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
    SIZE textSize = renderingContext->GetTextSize(inMemoryDC, text.c_str(), font);

    currentWidth = max(currentWidth, textSize.cx);
    currentHeight = max(currentHeight, textSize.cy);

    normalStateDC = renderingContext->CreateInMemoryDC(currentWidth, currentHeight);
    hoverStateDC = renderingContext->CreateInMemoryDC(currentWidth, currentHeight);
    renderingContext->ResizeDC(inMemoryDC, currentWidth, currentHeight);

    RenderStateDC(normalStateDC, normalColor);
    RenderStateDC(hoverStateDC, hoverColor);
}

void HoverTextButtonWindow::RenderStateDC(HDC hdc, Colors color)
{
    Renderer* renderer = new Renderer(hdc, renderingContext);
    renderer->PrintText(text.c_str(), font, color, 0, 0);
    delete renderer;
}

HoverTextButtonWindow::~HoverTextButtonWindow()
{
}