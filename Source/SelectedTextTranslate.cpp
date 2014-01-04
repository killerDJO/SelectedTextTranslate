#pragma region include and define

#include "Include\stdafx.h"
#include "Include\SelectedTextTranslate.h"
#include "Include\Translator.h"
#include "Include\ScrollbarManager.h"

#ifdef UNICODE
	#define stringcopy wcscpy
#else
	#define stringcopy strcpy
#endif

#define ID_TRAY_APP_ICON					5000
#define ID_TRAY_EXIT_CONTEXT_MENU_ITEM		3000
#define ID_TRAY_ABOUT_CONTEXT_MENU_ITEM		3001
#define ID_TRAY_TRANSLATE_CONTEXT_MENU_ITEM 3002
#define ID_TRANSLATE_HOTKEY					3003
#define WM_TRAYICON ( WM_USER + 1 )

#pragma endregion

#pragma region constants and globals

UINT WM_TASKBARCREATED = 0;

HWND g_hwnd;
HMENU g_menu;
HINSTANCE g_hinst;

NOTIFYICONDATA g_notifyIconData;

TranslateResult translateResult;

const UINT WINDOW_WIDTH = 300, WINDOW_HEIGHT = 400, WINDOW_PADDING = 5;

TCHAR buffer[65535];
TCHAR className[] = TEXT( "tray icon class" );

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void				DrawWindow();

#pragma endregion

void Minimize()
{
	Shell_NotifyIcon(NIM_ADD, &g_notifyIconData);
	ShowWindow(g_hwnd, SW_HIDE);
}

void InitNotifyIconData()
{
	memset( &g_notifyIconData, 0, sizeof( NOTIFYICONDATA ) ) ;
  
	g_notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
  
	g_notifyIconData.hWnd = g_hwnd;
	g_notifyIconData.uID = ID_TRAY_APP_ICON;
	g_notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE |  NIF_TIP;  
	g_notifyIconData.uCallbackMessage = WM_TRAYICON; 
	g_notifyIconData.hIcon = LoadIcon(g_hinst, MAKEINTRESOURCE(IDI_SELECTEDTEXTTRANSLATE));
	wcscpy_s(g_notifyIconData.szTip, TEXT("Selected text translate.."));
}

void ShowTranslateWindow()
{
	translateResult = Translator::GetTranslationResults();

	unsigned int horizontalChars = 0;
	unsigned int verticalChars = 4; // 2 chars for header
	for(size_t i = 0; i < translateResult.Categories.size(); ++i) {
		for (size_t j = 0; j <translateResult.Categories[i].Words.size(); ++j)
			horizontalChars = max(horizontalChars,translateResult.Categories[i].Words[j].Word.length());
		verticalChars += translateResult.Categories[i].Words.size() + 1;
	}

	horizontalChars += 4;
	horizontalChars = max(horizontalChars,translateResult.TranslatedWord.length() + 3);
	horizontalChars = max(horizontalChars,translateResult.Word.length() + 3);

	ScrollbarManager().xPos = 0;
	ScrollbarManager().yPos = 0;
	ScrollbarManager().InitializeScrollbars(g_hwnd,WINDOW_WIDTH,WINDOW_HEIGHT,horizontalChars,verticalChars);
	ShowWindow(g_hwnd, SW_SHOW);
	SwitchToThisWindow(g_hwnd,TRUE);	
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR args, int iCmdShow )
{
	HANDLE mutex = CreateMutex(NULL,FALSE,_T("Selected text translate"));
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return -1;
    }

	WM_TASKBARCREATED = RegisterWindowMessageA("TaskbarCreated");

	g_hinst = hInstance;

	WNDCLASSEX wnd = { 0 };
	wnd.hInstance = hInstance;
	wnd.lpszClassName = className;
	wnd.lpfnWndProc = WndProc;
	wnd.cbSize = sizeof (WNDCLASSEX);
	wnd.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SELECTEDTEXTTRANSLATE));
	wnd.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wnd.hCursor = LoadCursor (NULL, IDC_ARROW);
	wnd.hbrBackground = CreateSolidBrush(RGB(255,255,255));
  
	if (!RegisterClassEx(&wnd))
	{
		FatalAppExit( 0, TEXT("Couldn't register window class!") );
	}
  
	RECT workarea;	
    SystemParametersInfo(SPI_GETWORKAREA,0,&workarea,0);
	g_hwnd = CreateWindowEx( 
		WS_EX_TOOLWINDOW , 
		className, 
		NULL, 
		WS_SIZEBOX | WS_POPUP | WS_HSCROLL | WS_VSCROLL, 
		workarea.right - WINDOW_WIDTH - WINDOW_PADDING,  
		workarea.bottom - WINDOW_HEIGHT - WINDOW_PADDING,  
		WINDOW_WIDTH, 
		WINDOW_HEIGHT,		
		NULL, 
		NULL, 
		hInstance, 
		NULL);			

	RegisterHotKey(g_hwnd,ID_TRANSLATE_HOTKEY,MOD_CONTROL,0x54/*T*/);

	InitNotifyIconData();
	Minimize();
	
	MSG msg;
	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if( !IsWindowVisible( g_hwnd ) )
	{
		Shell_NotifyIcon(NIM_DELETE, &g_notifyIconData);
	}

	ReleaseMutex(mutex);
	CloseHandle(mutex);

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT* sizingBox;
	RECT workarea;	
	if (message== WM_TASKBARCREATED && !IsWindowVisible(g_hwnd))
	{
		Minimize();
		return 0;
	}
	int zDelta;
	switch (message)
	{
		case WM_CREATE:
			g_menu = CreatePopupMenu();			
			AppendMenu(g_menu, MF_STRING, ID_TRAY_TRANSLATE_CONTEXT_MENU_ITEM,  TEXT( "Translate from clipboard" ) );
			AppendMenu(g_menu, MF_STRING, ID_TRAY_ABOUT_CONTEXT_MENU_ITEM,  TEXT( "About" ) );
			AppendMenu(g_menu, MF_SEPARATOR, NULL, NULL);
			AppendMenu(g_menu, MF_STRING, ID_TRAY_EXIT_CONTEXT_MENU_ITEM,  TEXT( "Exit" ) );				
			break;  

		case WM_SIZING:			
			SystemParametersInfo(SPI_GETWORKAREA,0,&workarea,0);  		
			sizingBox = (RECT*)lParam;
			sizingBox->top = workarea.bottom - WINDOW_HEIGHT - WINDOW_PADDING;
			sizingBox->left = workarea.right - WINDOW_WIDTH - WINDOW_PADDING;
			sizingBox->bottom = sizingBox->top + WINDOW_HEIGHT;
			sizingBox->right = sizingBox->left + WINDOW_WIDTH;
			return TRUE;

		case WM_HSCROLL:
			ScrollbarManager().OnHorizontalScroll(hwnd, wParam, lParam);
			InvalidateRect(g_hwnd,NULL,TRUE);			
			break;

		case WM_VSCROLL:
			ScrollbarManager().OnVerticalScroll(hwnd, wParam, lParam);
			InvalidateRect(g_hwnd,NULL,TRUE);			
			break;
		case WM_MOUSEWHEEL:
			zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			if(zDelta < 0)
				SendMessage(g_hwnd,WM_VSCROLL,SB_LINEDOWN,NULL);
			else
				SendMessage(g_hwnd,WM_VSCROLL,SB_LINEUP,NULL);

		case WM_SYSCOMMAND:
			switch( wParam & 0xfff0 ) 
			{
				case SC_MINIMIZE:
				case SC_CLOSE:  
					Minimize(); 
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
				if (clicked == ID_TRAY_ABOUT_CONTEXT_MENU_ITEM)
				{
					DialogBox(g_hinst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
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
			Minimize();
			break;

		case WM_PAINT:
			DrawWindow();
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

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
	}
	return (INT_PTR)FALSE;
}

void WriteToBuffer(TCHAR* dest, string src)
{	
	TCHAR buffer[65535];
	int len = MultiByteToWideChar(20866,0,src.c_str(),-1,buffer,65535);

	dest[src.size()] = len;
	std::copy(buffer, buffer + len, dest);
}

void PrintText(HDC hdc, string text, HFONT font, int x, int y)
{
	SelectObject(hdc, font);
	WriteToBuffer(buffer,text);
	TextOut(hdc, x, y, buffer, _tcslen(buffer)); 
}

void DrawWindow()
{
	PAINTSTRUCT ps;	
	HDC hdc = BeginPaint(g_hwnd, &ps);
	
	RECT rect;

	int scrollX = int(ScrollbarManager().xPos * ScrollbarManager().xChar * -1);
	int scrollY = int(ScrollbarManager().yPos * ScrollbarManager().yChar * -1);
	
	const int padding = 15, lineHeight = 20;
	const int categoryMargin = 10;
	int curY = padding/2;

    long lfHeight = -MulDiv(lineHeight/2, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	long lfHeightHeader = -MulDiv(int(lineHeight*3/5.0), GetDeviceCaps(hdc, LOGPIXELSY), 72);
    HFONT fontNormal = CreateFont(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
	HFONT fontHeader = CreateFont(lfHeightHeader, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
	HFONT fontItalic = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
	
	HBRUSH ratingBrush = CreateSolidBrush(RGB(170,170,170));
		
	PrintText(hdc,translateResult.TranslatedWord,fontHeader,padding + scrollX, curY + scrollY);

	rect.right = 0;
	rect.left = WINDOW_WIDTH;
	rect.top = curY + int(5/4.0*lineHeight) + scrollY;
	rect.bottom = rect.top + 1;						
	FillRect(hdc,&rect,ratingBrush);

	curY += lineHeight + categoryMargin;

	auto categories = translateResult.Categories;
	if(categories.size() == 0)
	{
		SetTextColor(hdc, RGB(119, 119, 119));
		PrintText(hdc,translateResult.Word, fontNormal,padding + scrollX,curY + scrollY);
		curY += lineHeight;
	}
	for(size_t i = 0; i < categories.size(); ++i)
	{
		// Draw category header
		SIZE textSize;
		WriteToBuffer(buffer, translateResult.Word);		
		SelectObject(hdc, fontNormal);
		GetTextExtentPoint32(hdc,buffer,_tcslen(buffer),&textSize);		
		TextOut(hdc, padding + scrollX, curY + scrollY, buffer, _tcslen(buffer));

		SetTextColor(hdc, RGB(119, 119, 119));
		PrintText(hdc," - " + categories[i].CategoryName, fontItalic,padding + textSize.cx + scrollX + 2,curY + scrollY);

		// Draw words
		curY += lineHeight;
		SetTextColor(hdc,RGB(0, 0, 0));	
		auto words = categories[i].Words;
		for (size_t j = 0; j < words.size(); ++j)
		{
			PrintText(hdc, words[j].Word, fontNormal,padding * 3 + scrollX ,curY + scrollY);

			int k = words[j].Score >= 0.05 ? 0 : (words[j].Score >= 0.003 ? 1 : 2);
			int rateUnit = 8;
			
			RECT rect; 
			rect.right = padding + rateUnit * 3 + scrollX;
			rect.top = curY + lineHeight/3 + scrollY;
			rect.bottom = rect.top + lineHeight/3;						
			rect.left = padding + k * rateUnit + scrollX;

			FillRect(hdc,&rect,ratingBrush);

			curY += lineHeight;
		}

		curY += categoryMargin;
	}

	EndPaint(g_hwnd, &ps);
}