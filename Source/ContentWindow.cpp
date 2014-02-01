#include "stdafx.h"
#include "ContentWindow.h"
#include "TranslateResult.h"

ContentWindow::ContentWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y)
{
	this->parentWindow = parentWindow;
	this->hInstance = hInstance;
	this->initialX = x;
	this->initialY = y;

	const TCHAR* className = TEXT("STT_CONTENT");
	WNDCLASSEX wnd;
	if (!GetClassInfoEx(hInstance, className, &wnd))
	{
		wnd = { 0 };
		wnd.hInstance = hInstance;
		wnd.lpszClassName = className;
		wnd.lpfnWndProc = WndProc;
		wnd.cbSize = sizeof (WNDCLASSEX);
		wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
		wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

		if (!RegisterClassEx(&wnd))
		{
			FatalAppExit(0, TEXT("Couldn't register window class!"));
		}
	}

	this->hWindow = CreateWindowA(
		"STT_CONTENT",
		NULL,
		WS_CHILD | WS_VISIBLE,
		x,
		y,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		this->parentWindow,
		NULL,
		this->hInstance,
		this);

	this->InitializeFonts();
}

HWND ContentWindow::GetHandle()
{
	return this->hWindow;
}

HINSTANCE ContentWindow::GetInstance()
{
	return this->hInstance;
}

LRESULT CALLBACK ContentWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	ContentWindow* instance = (ContentWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	
	CREATESTRUCT* createstruct;
	RECT rcWindow;
	POINTS pos;

	switch (message)
	{
	case WM_NCCREATE:
	case WM_CREATE:
		createstruct = (CREATESTRUCT*)lParam;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)createstruct->lpCreateParams);
		return TRUE;

	case WM_MOVE:
		pos = MAKEPOINTS(lParam);
		GetWindowRect(hWnd, &rcWindow);
		MoveWindow(hWnd, pos.x, pos.y, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, FALSE);
		InvalidateRect(hWnd, NULL, FALSE);
		return TRUE;
	
	case WM_NCPAINT:
	case WM_PAINT:
		instance->Draw();
		break;

	case WM_SHOWWINDOW:
		instance->Draw();
		break;

	case WM_HSCROLL:
	case WM_VSCROLL:
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void ContentWindow::RenderResult(TranslateResult translateResult)
{
	this->translateResult = translateResult;
	MoveWindow(this->hWindow, this->initialX, this->initialY, WINDOW_WIDTH, WINDOW_HEIGHT, TRUE);
	InvalidateRect(this->hWindow, NULL, TRUE);
	this->Draw();
}

void ContentWindow::InitializeFonts()
{
	HDC hdc = GetDC(hWindow);

	long lfHeight = -MulDiv(LINE_HEIGHT / 2, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	long lfHeightHeader = -MulDiv(int(LINE_HEIGHT * 3 / 5.0), GetDeviceCaps(hdc, LOGPIXELSY), 72);
	long lfHeightSmall = -MulDiv(int(LINE_HEIGHT * 3 / 7.0), GetDeviceCaps(hdc, LOGPIXELSY), 72);

	this->fontNormal = CreateFont(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
	this->fontHeader = CreateFont(lfHeightHeader, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
	this->fontItalic = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
	this->fontSmall = CreateFont(lfHeightSmall, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
}

void ContentWindow::Draw()
{
}

ContentWindow::~ContentWindow()
{	
}