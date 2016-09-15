#include "Windows\Buttons\HoverTextButtonWindow.h"

HoverTextButtonWindow::HoverTextButtonWindow(Renderer* renderer, HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, HFONT font, Colors normalColor, Colors hoverColor, wstring text, function<void()> clickCallback)
    : HoverButtonWindow(renderer, parentWindow, hInstance, x, y, 0, 0, clickCallback)
{
    this->font = font;
    this->normalColor = normalColor;
    this->hoverColor = hoverColor;
    this->text = text;
}

void HoverTextButtonWindow::InitializeInMemoryDC()
{
    inMemoryDC = renderer->CreateInMemoryDC(width, height);
    SIZE textSize = renderer->GetTextSize(inMemoryDC, text.c_str(), font);

    width = max(width, (DWORD)textSize.cx);
    height = max(height, (DWORD)textSize.cy);

    normalStateDC = renderer->CreateInMemoryDC(width, height);
    hoverStateDC = renderer->CreateInMemoryDC(width, height);
    renderer->ResizeDC(inMemoryDC, width, height);
}

void HoverTextButtonWindow::RenderStatesDC()
{
    RenderStateDC(normalStateDC, normalColor);
    RenderStateDC(hoverStateDC, hoverColor);
}

void HoverTextButtonWindow::RenderStateDC(HDC hdc, Colors color)
{
    POINT bottomRight;
    renderer->PrintText(hdc, text.c_str(), font, color, 0, 0, &bottomRight);
}

HoverTextButtonWindow::~HoverTextButtonWindow()
{
}