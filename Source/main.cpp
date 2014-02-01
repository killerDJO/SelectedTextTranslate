#include "stdafx.h"
#include "Translator.h"
#include "MainWindow.h"

#define ID_TRAY_EXIT_CONTEXT_MENU_ITEM		3000
#define ID_TRAY_TRANSLATE_CONTEXT_MENU_ITEM 3002
#define ID_TRANSLATE_HOTKEY					3003

LRESULT CALLBACK  WndProc(HWND, UINT, WPARAM, LPARAM);

UINT WM_TASKBARCREATED;
HMENU g_menu;
MainWindow* g_mainWindow;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR args, int iCmdShow )
{ 
	HANDLE mutex = CreateMutex(NULL,FALSE,_T("Selected text translate"));
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		FatalAppExit(0, TEXT("App already started!"));
    }
	WM_TASKBARCREATED = RegisterWindowMessageA("TaskbarCreated");

	g_mainWindow = new MainWindow(hInstance, WndProc);
	
	RegisterHotKey(g_mainWindow->GetHandle(), ID_TRANSLATE_HOTKEY, MOD_CONTROL, 0x54/*T*/);
	
	MSG msg;
	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	ReleaseMutex(mutex);
	CloseHandle(mutex);
	delete g_mainWindow;

	return msg.wParam;
}

void ShowTranslateWindow()
{
	TranslateResult translateResult = Translator::TranslateSelectedText();
	g_mainWindow->SetTranslateResult(translateResult);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_TASKBARCREATED && !IsWindowVisible(g_mainWindow->GetHandle()))
	{
		g_mainWindow->Minimize();
		return 0;
	}
	int zDelta;
	switch (message)
	{
		case WM_CREATE:
			g_menu = CreatePopupMenu();			
			AppendMenu(g_menu, MF_STRING, ID_TRAY_TRANSLATE_CONTEXT_MENU_ITEM,  TEXT( "Translate from clipboard" ) );
			AppendMenu(g_menu, MF_SEPARATOR, NULL, NULL);
			AppendMenu(g_menu, MF_STRING, ID_TRAY_EXIT_CONTEXT_MENU_ITEM,  TEXT( "Exit" ) );				
			break;  

		case WM_SIZING:			
			return g_mainWindow->ProcessSizing(wParam, lParam);
		
		case WM_SETCURSOR:
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			return TRUE;

		case WM_HSCROLL:
			g_mainWindow->ProcessHorizontalScroll(wParam, lParam);
			break;

		case WM_VSCROLL:
			g_mainWindow->ProcessVerticalScroll(wParam, lParam);
			break;

		case WM_MOUSEWHEEL:
			zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			if(zDelta < 0)
				SendMessage(g_mainWindow->GetHandle(), WM_VSCROLL, SB_LINEDOWN, NULL);
			else
				SendMessage(g_mainWindow->GetHandle(), WM_VSCROLL, SB_LINEUP, NULL);

		case WM_SYSCOMMAND:
			switch( wParam & 0xfff0 ) 
			{
				case SC_MINIMIZE:
				case SC_CLOSE:  
					g_mainWindow->Minimize();
					return 0;				
			}
			return DefWindowProc(hwnd, message, wParam, lParam);

		case WM_TRAYICON:  
			switch(wParam)
			{
				case ID_TRAY_APP_ICON:
				break;
			}
			if(lParam == WM_LBUTTONUP)
			{
				ShowTranslateWindow();
			}
			if (lParam == WM_RBUTTONUP)
			{
				POINT curPoint;
				GetCursorPos( &curPoint );
				SetForegroundWindow(hwnd); 
				UINT clicked = TrackPopupMenu(g_menu, TPM_RETURNCMD | TPM_NONOTIFY, curPoint.x, curPoint.y, 0, hwnd, NULL);
				if (clicked == ID_TRAY_EXIT_CONTEXT_MENU_ITEM)
				{
					PostQuitMessage( 0 );
				}
				if (clicked == ID_TRAY_TRANSLATE_CONTEXT_MENU_ITEM)
				{
					ShowTranslateWindow();
				}
			}		
			break;
		
		case WM_HOTKEY:
			if(wParam == ID_TRANSLATE_HOTKEY)
			{
				ShowTranslateWindow();
			}
			break;

		case WM_KILLFOCUS:
			if (GetParent((HWND)wParam) != g_mainWindow->GetHandle())
			{
				g_mainWindow->Minimize();
			}			
			break;

		case WM_DESTROY:
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
	
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}