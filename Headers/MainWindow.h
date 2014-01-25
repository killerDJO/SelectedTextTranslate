#pragma once
#include "stdafx.h"
#include "TranslateResult.h"

#ifdef UNICODE
	#define stringcopy wcscpy
#else
	#define stringcopy strcpy
#endif

#define ID_TRAY_APP_ICON	5000
#define WM_TRAYICON ( WM_USER + 1 )

class MainWindow
{
private:
	const UINT		WINDOW_WIDTH	= 300;
	const UINT		WINDOW_HEIGHT	= 400;
	const UINT		WINDOW_PADDING	= 5;

	const double	xChar = 8;
	const double	yChar = 20;

	TCHAR buffer[65535];

	NOTIFYICONDATA	notifyIconData;
	HWND			hWindow;
	HINSTANCE		hInstance;

	TranslateResult translateResult;

	int scrollOffsetX = 0;
	int scrollOffsetY = 0;

	void InitNotifyIconData();
	void InitializeScrollbars(UINT horizontalChars, UINT verticalChars);
	void WriteToBuffer(TCHAR* dest, string src);
	void PrintText(HDC hdc, string text, HFONT font, int x, int y);
	
public:
	MainWindow(HINSTANCE hInstance, WNDPROC wndProc);
	~MainWindow();

	HWND GetHandle();

	void Minimize();
	void SetTranslateResult(TranslateResult translateResult);
	void ProcessVerticalScroll(WPARAM wParam, LPARAM lParam);
	void ProcessHorizontalScroll(WPARAM wParam, LPARAM lParam);
	UINT ProcessSizing(WPARAM wParam, LPARAM lParam);
	void DrawWindow();
};