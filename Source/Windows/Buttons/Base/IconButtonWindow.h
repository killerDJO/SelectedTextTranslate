#pragma once
#include "Helpers\Utilities.h"

class IconButtonWindow
{
protected:
	HINSTANCE hInstance;
	HWND hWindow;
	HWND hParent;

	HBITMAP imageNormal, imageHover;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
public:
	IconButtonWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
	~IconButtonWindow();

	HWND GetHandle();
	HINSTANCE GetInstance();

	HBITMAP GetNormalImage();
	HBITMAP GetHoverImage();

	void Show();
	void Hide();
};

