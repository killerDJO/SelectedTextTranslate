#pragma once
#include "Helpers\Utilities.h"
#include "Windows\Buttons\Base\IconButtonWindow.h"

class TranslateButtonWindow : public IconButtonWindow
{
protected:
	void InitializeBitmaps();

public:
	TranslateButtonWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
	~TranslateButtonWindow();
};