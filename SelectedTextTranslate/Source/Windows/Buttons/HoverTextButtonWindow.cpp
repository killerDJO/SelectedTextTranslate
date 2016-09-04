#include "Windows\Buttons\HoverTextButtonWindow.h"

HoverTextButtonWindow::HoverTextButtonWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, HFONT font, COLORREF normalColor, COLORREF hoverColor, wstring text, function<void()> clickCallback)
    : HoverButtonWindow(parentWindow, hInstance, x, y, 1, 1, clickCallback)
{
    this->font = font;
    this->normalColor = normalColor;
    this->hoverColor = hoverColor;
    this->text = text;
}

void HoverTextButtonWindow::RenderStatesHDC()
{
    SIZE textSize = GetTextSize(this->inMemoryHDC, this->text.c_str(), this->font);

    this->width = max(this->width, (DWORD)textSize.cx);
    this->height = max(this->height, (DWORD)textSize.cy);

    ResizeHDC(this->normalStateHDC, this->width, this->height);
    ResizeHDC(this->hoverStateHDC, this->width, this->height);
    ResizeHDC(this->inMemoryHDC, this->width, this->height);

    RenderStateHDC(this->normalStateHDC, this->normalColor);
    RenderStateHDC(this->hoverStateHDC, this->hoverColor);
}

void HoverTextButtonWindow::RenderStateHDC(HDC hdc, COLORREF color)
{
    POINT bottomRight;
    PrintText(hdc, this->text.c_str(), this->font, color, 0, 0, &bottomRight);
}

HoverTextButtonWindow::~HoverTextButtonWindow()
{
}