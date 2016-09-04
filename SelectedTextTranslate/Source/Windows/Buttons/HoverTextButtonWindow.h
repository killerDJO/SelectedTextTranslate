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

    void RenderStateDC(HDC hdc, COLORREF color);

public:
    HoverTextButtonWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, HFONT font, COLORREF normalColor, COLORREF hoverColor, wstring text, function<void()> clickCallback);
    ~HoverTextButtonWindow();
};