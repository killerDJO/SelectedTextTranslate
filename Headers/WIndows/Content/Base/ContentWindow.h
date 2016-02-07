#pragma once
#include "PrecompiledHeaders\stdafx.h"
#include "Entities\TranslateResult.h"
#include "Helpers\Utilities.h"

class ContentWindow
{
protected:
	const	UINT	FONT_HEIGHT = 20;
	UINT	PADDING_X;
	UINT	PADDING_Y;
	UINT	LINE_HEIGHT;
	double	kX;
	double	kY;

	int initialX, initialY;
	int width, height;

	HWND parentWindow;
	HWND hWindow;
	HINSTANCE hInstance;
	HDC inMemoryHDC;
	
	COLORREF COLOR_GRAY;
	COLORREF COLOR_BLACK;

	HFONT fontNormal, fontHeader, fontItalic, fontSmall;
	HBRUSH grayBrush;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void ComputeParameters();
	virtual void InitializeFonts();
	virtual void InitializeBrushes();
	virtual void InitializeInMemoryDC();
	virtual POINT RenderDC();
	virtual void ResetWindow(POINT bottomRight);
	virtual DWORD AdjustToResolution(double value, double k);

public:
	ContentWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
	~ContentWindow();

	virtual HWND GetHandle();
	virtual HINSTANCE GetInstance();

	virtual POINT RenderResult();
	virtual void Draw();

	void Show();
	void Hide();
};