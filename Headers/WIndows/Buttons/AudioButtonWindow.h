#pragma once
#include "Entities\TranslateResult.h"
#include "Helpers\Utilities.h"

class AudioButtonWindow
{
private:

	HINSTANCE hInstance;
	HWND hWindow;
	HWND hParent;

	HBITMAP imageNormal, imageHover;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	void InitializeBitmaps();
public:
	AudioButtonWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y);
	~AudioButtonWindow();

	HWND GetHandle();
	HINSTANCE GetInstance();
	
	HBITMAP GetNormalImage();
	HBITMAP GetHoverImage();

	void Show();
	void Hide();
};