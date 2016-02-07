#pragma once
#include "Entities\TranslateResult.h"
#include "Helpers\Utilities.h"
#include "Windows\Buttons\Base\IconButtonWindow.h"

class AudioButtonWindow : public IconButtonWindow
{
protected:
	void InitializeBitmaps();

public:
	AudioButtonWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
	~AudioButtonWindow();
};