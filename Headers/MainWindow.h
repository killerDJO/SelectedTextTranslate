#pragma once
#include "stdafx.h"
#include "TranslateResult.h"
#include "HeaderWindow.h"
#include "TranslateResultWindow.h"

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
	const UINT		WINDOW_PADDING  = 5;

	const double	SCROLL_CHAR_X = 8;
	const double	SCROLL_CHAR_Y = 20;

	NOTIFYICONDATA	notifyIconData;
	HWND			hWindow;
	HINSTANCE		hInstance;

	HeaderWindow * headerWindow;
	TranslateResultWindow  * translateResultWindow;

	TranslateResult translateResult;

	void InitNotifyIconData();
	void InitializeScrollbars(UINT horizontalChars, UINT verticalChars);

public:
	MainWindow(HINSTANCE hInstance, WNDPROC wndProc);
	~MainWindow();

	HWND GetHandle();
	HINSTANCE GetInstance();

	void Minimize();
	void Maximize();

	void SetTranslateResult(TranslateResult translateResult);
	
	void ProcessVerticalScroll(WPARAM wParam, LPARAM lParam);
	void ProcessHorizontalScroll(WPARAM wParam, LPARAM lParam);
	UINT ProcessSizing(WPARAM wParam, LPARAM lParam);
};