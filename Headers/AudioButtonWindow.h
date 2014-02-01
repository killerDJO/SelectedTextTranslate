#pragma once
#include "TranslateResult.h"
#include "Utilities.h"

class AudioButtonWindow
{
private:

	HINSTANCE hInstance;
	HWND hWindow;
	HWND hParent;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	
public:
	AudioButtonWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y);
	~AudioButtonWindow();

	HWND GetHandle();
	HINSTANCE GetInstance();
};