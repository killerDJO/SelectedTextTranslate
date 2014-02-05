#pragma once
#include "PrecompiledHeaders\stdafx.h"
#include "Entities\TranslateResult.h"
#include "Windows\Content\HeaderWindow.h"
#include "Windows\Content\TranslateResultWindow.h"

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
	static const UINT	WINDOW_WIDTH	= 300;
	static const UINT	WINDOW_HEIGHT = 400;
	static const UINT	WINDOW_PADDING = 5;

	static const int	SCROLL_CHAR_X = 8;
	static const int	SCROLL_CHAR_Y = 20;

	NOTIFYICONDATA	notifyIconData;
	HWND			hWindow;
	HINSTANCE		hInstance;

	HeaderWindow * headerWindow;
	TranslateResultWindow  * translateResultWindow;

	TranslateResult translateResult;

	void InitNotifyIconData();
	void InitializeScrollbars(int contentWidth, int contentHeight);

public:
	MainWindow(HINSTANCE hInstance, WNDPROC wndProc);
	~MainWindow();

	HWND GetHandle();
	HINSTANCE GetInstance();

	void Minimize();
	void Maximize();

	void SetTranslateResult(TranslateResult translateResult, BOOL maximize);
	void PlayText();
	
	void ProcessVerticalScroll(WPARAM wParam, LPARAM lParam);
	void ProcessHorizontalScroll(WPARAM wParam, LPARAM lParam);
	UINT ProcessSizing(WPARAM wParam, LPARAM lParam);
};