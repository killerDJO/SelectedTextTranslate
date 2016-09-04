#include "Windows\Buttons\HoverTextButtonWindow.h"

HoverTextButtonWindow::HoverTextButtonWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, HFONT font, COLORREF normalColor, COLORREF hoverColor, wstring text, function<void()> clickCallback)
    : HoverButtonWindow(parentWindow, hInstance, x, y, 1, 1, clickCallback)
{
    this->font = font;
    this->normalColor = normalColor;
    this->hoverColor = hoverColor;
    this->text = text;
}

void HoverTextButtonWindow::RenderStatesDC()
{
    SIZE textSize = GetTextSize(inMemoryDC, text.c_str(), font);

    width = max(width, (DWORD)textSize.cx);
    height = max(height, (DWORD)textSize.cy);

    ResizeDC(normalStateDC, width, height);
    ResizeDC(hoverStateDC, width, height);
    ResizeDC(inMemoryDC, width, height);

    RenderStateDC(this->normalStateDC, this->normalColor);
    RenderStateDC(this->hoverStateDC, this->hoverColor);
}

void HoverTextButtonWindow::RenderStateDC(HDC hdc, COLORREF color)
{
    POINT bottomRight;
    PrintText(hdc, text.c_str(), font, color, 0, 0, &bottomRight);
}

HoverTextButtonWindow::~HoverTextButtonWindow()
{
}