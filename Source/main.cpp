#include "PrecompiledHeaders\stdafx.h"
#include "TranslateEngine\Translator.h"
#include "Windows\MainWindow.h"

#define ID_TRAY_EXIT_CONTEXT_MENU_ITEM			3000
#define ID_TRAY_TRANSLATE_CONTEXT_MENU_ITEM		3002
#define ID_TRANSLATE_HOTKEY						3003
#define ID_PLAYTEXT_HOTKEY						3004
#define ID_TRAY_DICTIONARY_CONTEXT_MENU_ITEM	3005

LRESULT CALLBACK  WndProc(HWND, UINT, WPARAM, LPARAM);

UINT WM_TASKBARCREATED;
HMENU g_menu;
MainWindow* g_mainWindow;

void AttachConsole()
{
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR args, int iCmdShow )
{ 
	//AttachConsole();

	HANDLE mutex = CreateMutex(NULL,FALSE,_T("Selected text translate"));
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		FatalAppExit(0, TEXT("App already started!"));
    }
	WM_TASKBARCREATED = RegisterWindowMessageA("TaskbarCreated");

	g_mainWindow = new MainWindow(hInstance, WndProc);
	
	RegisterHotKey(g_mainWindow->GetHandle(), ID_TRANSLATE_HOTKEY, MOD_CONTROL, 0x54/*T*/);
	RegisterHotKey(g_mainWindow->GetHandle(), ID_PLAYTEXT_HOTKEY, MOD_CONTROL, 0x52/*R*/);
	
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
	g_mainWindow->SetTranslateResult(translateResult, TRUE);
}

void TranslateRecord(int idx)
{
	vector<LogRecord> records = Logger::GetRecords();
	string word = records[idx].Word;
	TranslateResult translateResult = Translator::TranslateSentence(word);
	g_mainWindow->SetTranslateResult(translateResult, TRUE);
}

void ShowDictionaryWindow()
{
	vector<LogRecord> records = Logger::GetRecords();
	g_mainWindow->RenderDictionary(records);
}

void PlayText()
{
	TranslateResult translateResult = Translator::TranslateSelectedText();
	g_mainWindow->SetTranslateResult(translateResult, FALSE);
	g_mainWindow->PlayText();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_TASKBARCREATED && !IsWindowVisible(g_mainWindow->GetHandle()))
	{
		g_mainWindow->Minimize();
		return 0;
	}

	HWND windowWithFocus, currentWindow;

	int zDelta;
	switch (message)
	{
	
		case WM_NOTIFY:{
			if (wParam == WM_TRANLSATE_LOG_RECORD) 
			{
				TranslateRecord(lParam);
			}

			break;
		}
		case WM_CREATE:
		{
			g_menu = CreatePopupMenu();
			AppendMenu(g_menu, MF_STRING, ID_TRAY_TRANSLATE_CONTEXT_MENU_ITEM, TEXT("Translate from clipboard"));
			AppendMenu(g_menu, MF_STRING, ID_TRAY_DICTIONARY_CONTEXT_MENU_ITEM, TEXT("Dictionary"));
			AppendMenu(g_menu, MF_SEPARATOR, NULL, NULL);
			AppendMenu(g_menu, MF_STRING, ID_TRAY_EXIT_CONTEXT_MENU_ITEM, TEXT("Exit"));

			break;
		}

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
				if (clicked == ID_TRAY_DICTIONARY_CONTEXT_MENU_ITEM)
				{
					ShowDictionaryWindow();
				}
			}		
			break;
		
		case WM_HOTKEY:
			if(wParam == ID_TRANSLATE_HOTKEY)
			{
				ShowTranslateWindow();
			}
			if (wParam == ID_PLAYTEXT_HOTKEY)
			{
				PlayText();
			}
			break;

		case WM_KILLFOCUS:
			windowWithFocus = GetParent((HWND)wParam);
			currentWindow = g_mainWindow->GetHandle();
			if (windowWithFocus != currentWindow)
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