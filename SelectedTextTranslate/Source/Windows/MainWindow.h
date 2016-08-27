#pragma once
#include "PrecompiledHeaders\stdafx.h"
#include "Entities\TranslateResult.h"
#include "Entities\LogRecord.h"
#include "Loggers\DictionaryLogger.h"
#include "Windows\Content\HeaderWindow.h"
#include "Windows\Content\TranslateResultWindow.h"
#include "Windows\Content\DictionaryWindow.h"

#ifdef UNICODE
	#define stringcopy wcscpy
#else
	#define stringcopy strcpy
#endif

#define ID_TRAY_APP_ICON	5000
#define WM_TRAYICON ( WM_USER + 1 )

class TranslateResultWindow;

class MainWindow
{
private:
	UINT	WINDOW_WIDTH;
	UINT	WINDOW_HEIGHT;
	UINT	WINDOW_PADDING;
	double	kX;
	double	kY;

	static const int	SCROLL_CHAR_X	= 8;
	static const int	SCROLL_CHAR_Y	= 20;

	NOTIFYICONDATA	notifyIconData;
	HWND			hWindow;
	HINSTANCE		hInstance;

	HeaderWindow * headerWindow;
	TranslateResultWindow * translateResultWindow;
	DictionaryWindow * dictionaryWindow;

	TranslateResult translateResult;

	void ComputeWindowDimensions(RECT workarea);
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
	void Render(TranslateResult translateResult, int vScroll = 0);
	void RenderDictionary(vector<LogRecord> records);
	void PlayText();
	
	void ProcessVerticalScroll(WPARAM wParam, LPARAM lParam);
	void ProcessHorizontalScroll(WPARAM wParam, LPARAM lParam);
	UINT ProcessSizing(WPARAM wParam, LPARAM lParam);
};