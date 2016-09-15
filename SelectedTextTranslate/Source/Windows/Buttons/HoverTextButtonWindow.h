#pragma once
#include "Windows\Buttons\Base\HoverButtonWindow.h"

class HoverTextButtonWindow : public HoverButtonWindow
{
protected:
    void RenderStatesDC() override;

private:
    HFONT font;
    Colors normalColor;
    Colors hoverColor;
    wstring text;

    void InitializeInMemoryDC() override;

    void RenderStateDC(HDC hdc, Colors color);

public:
    HoverTextButtonWindow(Renderer* renderer, HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, HFONT font, Colors normalColor, Colors hoverColor, wstring text, function<void()> clickCallback);
    ~HoverTextButtonWindow();
};