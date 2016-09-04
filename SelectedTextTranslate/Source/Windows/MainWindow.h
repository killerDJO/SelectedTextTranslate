#pragma once
#include "Windows\Content\HeaderWindow.h"
#include "Windows\Content\DictionaryWindow.h"
#include "Windows\Content\TranslateResultWindow.h"

#define ID_TRAY_APP_ICON	5000
#define WM_TRAYICON ( WM_USER + 1 )

#define ID_TRAY_EXIT_CONTEXT_MENU_ITEM			3000
#define ID_TRAY_TRANSLATE_CONTEXT_MENU_ITEM		3002
#define ID_TRANSLATE_HOTKEY						3003
#define ID_PLAYTEXT_HOTKEY						3004
#define ID_TRAY_DICTIONARY_CONTEXT_MENU_ITEM	3005

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

	static UINT WM_TASKBARCREATED;
	HMENU menu;

	NOTIFYICONDATA	notifyIconData;
	HWND			hWindow;
	HINSTANCE		hInstance;

	HeaderWindow* headerWindow;
	TranslateResultWindow* translateResultWindow;
	DictionaryWindow* dictionaryWindow;

	AppModel* appModel;

	void ComputeWindowDimensions(RECT workarea);
	void InitNotifyIconData();
	void InitializeScrollbars(int contentWidth, int contentHeight);

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void ProcessVerticalScroll(WPARAM wParam, LPARAM lParam);
	void ProcessHorizontalScroll(WPARAM wParam, LPARAM lParam);
	UINT ProcessSizing(WPARAM wParam, LPARAM lParam);

public:
	MainWindow(HINSTANCE hInstance, AppModel* appModel);
	~MainWindow();

	void Minimize();
	void Maximize();

	void ShowTranslateResultView(bool preserveScroll = false);
	void ShowDictionaryView();

	HWND GetHandle();
	HINSTANCE GetInstance();
};