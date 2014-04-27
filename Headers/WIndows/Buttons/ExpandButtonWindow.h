#pragma once
#include "Entities\TranslateResult.h"
#include "Helpers\Utilities.h"

class ExpandButtonWindow
{
private:
	HINSTANCE hInstance;
	HWND hWindow;
	HWND hParent;
	HFONT font;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	void InitializeFonts();
public:
	ExpandButtonWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height, DWORD hiddenCount, bool expandState);
	~ExpandButtonWindow();

	HWND GetHandle();
	HINSTANCE GetInstance();

	void Destroy();
};