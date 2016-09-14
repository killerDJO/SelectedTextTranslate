#pragma once
#include "Windows\Buttons\Base\HoverButtonWindow.h"

class HoverTextButtonWindow : public HoverButtonWindow
{
protected:
    void RenderStatesDC() override;

private:
    HFONT font;
    COLORREF normalColor;
    COLORREF hoverColor;
    wstring text;

    void InitializeInMemoryDC() override;

    void RenderStateDC(HDC hdc, COLORREF color);

public:
    HoverTextButtonWindow(Renderer* renderer, HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, HFONT font, COLORREF normalColor, COLORREF hoverColor, wstring text, function<void()> clickCallback);
    ~HoverTextButtonWindow();
};