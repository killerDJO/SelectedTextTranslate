#pragma once
#include "Helpers\Utilities.h"
#include "Windows\Buttons\Base\IconButtonWindow.h"

class RemoveButtonWindow : public IconButtonWindow
{
protected:
	void InitializeBitmaps();

public:
	RemoveButtonWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
	~RemoveButtonWindow();
};