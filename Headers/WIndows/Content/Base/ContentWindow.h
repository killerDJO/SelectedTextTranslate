#pragma once
#include "PrecompiledHeaders\stdafx.h"
#include "Entities\TranslateResult.h"
#include "Helpers\Utilities.h"

class ContentWindow
{
protected:
	const int PADDING = 15;
	const int LINE_HEIGHT = 20;

	int initialX, initialY;
	int width, height;

	HWND parentWindow;
	HWND hWindow;
	HINSTANCE hInstance;
	HDC inMemoryHDC;

	TranslateResult translateResult;
	
	COLORREF COLOR_GRAY = RGB(119, 119, 119);
	COLORREF COLOR_BLACK = RGB(0, 0, 0);

	HFONT fontNormal, fontHeader, fontItalic, fontSmall;
	HBRUSH grayBrush;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void InitializeFonts();
	virtual void InitializeBrushes();
	virtual void InitializeInMemoryDC();
	virtual POINT RenderDC();
	virtual void ResetWindow(POINT bottomRight);

public:
	ContentWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
	~ContentWindow();

	virtual HWND GetHandle();
	virtual HINSTANCE GetInstance();

	virtual POINT RenderResult(TranslateResult translateResult);
	virtual void Draw();
};