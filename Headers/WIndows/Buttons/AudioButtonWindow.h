#pragma once
#include "Entities\TranslateResult.h"
#include "Helpers\Utilities.h"

#define MOUSEEVENTF_FROMTOUCH 0xFF515700

class AudioButtonWindow
{
private:
	HINSTANCE hInstance;
	HWND hWindow;
	HWND hParent;

	HBITMAP imageNormal, imageHover;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	static void OnTouch(HWND hWnd, WPARAM wParam, LPARAM lParam, AudioButtonWindow* instance);
	void InitializeBitmaps();
public:
	AudioButtonWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
	~AudioButtonWindow();

	HWND GetHandle();
	HINSTANCE GetInstance();
	
	HBITMAP GetNormalImage();
	HBITMAP GetHoverImage();

	void Show();
	void Hide();
};