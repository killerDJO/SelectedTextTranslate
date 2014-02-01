#pragma once
#include "ContentWindow.h"
#include "TranslateResult.h"
#include "Utilities.h"

class ContentWindow
{
protected:
	const int PADDING = 15;
	const int LINE_HEIGHT = 20;

	int initialX;
	int initialY;
	int width;
	int height;

	HWND parentWindow;
	HWND hWindow;
	HINSTANCE hInstance;

	HDC inMemoryHDC;

	TranslateResult translateResult;
	
	HFONT fontNormal, fontHeader, fontItalic, fontSmall;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void InitializeFonts();
	virtual void InitializeInMemoryDC();
	virtual void RenderDC();

public:
	ContentWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
	~ContentWindow();

	virtual HWND GetHandle();
	virtual HINSTANCE GetInstance();

	virtual void RenderResult(TranslateResult translateResult);
	virtual void Draw();
};