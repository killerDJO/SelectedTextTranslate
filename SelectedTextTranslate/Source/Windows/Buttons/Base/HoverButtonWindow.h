#pragma once
#include "Helpers\Utilities.h"
#include "Windows\Base\WindowBase.h"

class HoverButtonWindow : public WindowBase
{
protected:
	HDC hoverStateHDC;
	HDC normalStateHDC;

	POINT RenderDC() override;
	virtual void RenderStatesHDC() = 0;

private:
	function<void()> clickCallback;

	bool isHovered;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	HoverButtonWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height, function<void()> clickCallback);
	~HoverButtonWindow();

	void Initialize() override;
};