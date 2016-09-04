#pragma once
#include "Windows\Buttons\Base\HoverButtonWindow.h"

class HoverTextButtonWindow : public HoverButtonWindow
{
protected:
	void RenderStatesHDC() override;

private:
	HFONT font;
	COLORREF normalColor;
	COLORREF hoverColor;
	wstring text;

	void RenderStateHDC(HDC hdc, COLORREF color);

public:
	HoverTextButtonWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, HFONT font, COLORREF normalColor, COLORREF hoverColor, wstring text, function<void()> clickCallback);
	~HoverTextButtonWindow();
};