#pragma once
#include "ContentWindow.h"
#include "TranslateResult.h"
#include "Utilities.h"

class ContentWindow
{
protected:
	const int WINDOW_WIDTH = 500;
	const int WINDOW_HEIGHT = 2000;
	const int PADDING = 15;
	const int LINE_HEIGHT = 20;

	int initialX;
	int initialY;
	
	TranslateResult translateResult;

	HWND parentWindow;
	HWND hWindow;
	HINSTANCE hInstance;

	HDC inMemoryHDC;
	
	HFONT fontNormal, fontHeader, fontItalic, fontSmall;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void InitializeFonts();
	
public:
	ContentWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y);
	~ContentWindow();

	HWND GetHandle();
	HINSTANCE GetInstance();

	virtual void RenderResult(TranslateResult translateResult);
	virtual void Draw();
};