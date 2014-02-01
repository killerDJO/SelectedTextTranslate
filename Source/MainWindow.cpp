#include "stdafx.h"
#include "MainWindow.h"

MainWindow::MainWindow(HINSTANCE hInstance, WNDPROC wndProc)
{
	this->hInstance = hInstance;

	const TCHAR* className = TEXT("STT_MAIN");

	WNDCLASSEX wnd = { 0 };
	wnd.hInstance = hInstance;
	wnd.lpszClassName = className;
	wnd.lpfnWndProc = wndProc;
	wnd.cbSize = sizeof (WNDCLASSEX);
	wnd.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SELECTEDTEXTTRANSLATE));
	wnd.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SELECTEDTEXTTRANSLATE));
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
	
	if (!RegisterClassEx(&wnd))
	{
		FatalAppExit(0, TEXT("Couldn't register window class!"));
	}

	RECT workarea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);
	this->hWindow = CreateWindowEx(
		WS_EX_TOOLWINDOW,
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

	this->InitNotifyIconData();

	this->translateResultWindow = new TranslateResultWindow(this->hWindow, this->hInstance, 0, 50);
	this->headerWindow = new HeaderWindow(this->hWindow, this->hInstance, 0, 0);

	Minimize();
}

HWND MainWindow::GetHandle()
{
	return this->hWindow;
}
HINSTANCE MainWindow::GetInstance()
{
	return this->hInstance;
}

void MainWindow::InitNotifyIconData()
{
	memset(&this->notifyIconData, 0, sizeof(NOTIFYICONDATA));

	this->notifyIconData.cbSize = sizeof(NOTIFYICONDATA);

	this->notifyIconData.hWnd = this->hWindow;
	this->notifyIconData.uID = ID_TRAY_APP_ICON;
	this->notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	this->notifyIconData.uCallbackMessage = WM_TRAYICON;
	this->notifyIconData.hIcon = LoadIcon(this->hInstance, MAKEINTRESOURCE(IDI_SELECTEDTEXTTRANSLATE));
	wcscpy_s(this->notifyIconData.szTip, TEXT("Selected text translate.."));
}

void MainWindow::Minimize()
{
	Shell_NotifyIcon(NIM_ADD, &this->notifyIconData);
	ShowWindow(this->hWindow, SW_HIDE);
}
void MainWindow::Maximize()
{
	UINT horizontalChars = 0;
	UINT verticalChars = 4;

	for (size_t i = 0; i < this->translateResult.TranslateCategories.size(); ++i) {
		for (size_t j = 0; j <this->translateResult.TranslateCategories[i].Entries.size(); ++j)
			horizontalChars = max(horizontalChars, _tcslen(this->translateResult.TranslateCategories[i].Entries[j].Word));
		verticalChars += this->translateResult.TranslateCategories[i].Entries.size() + 2;
	}

	horizontalChars += 4;
	horizontalChars = max(horizontalChars, _tcslen(this->translateResult.Sentence.Origin) + 3);
	horizontalChars = max(horizontalChars, _tcslen(this->translateResult.Sentence.Translation) + 3);

	this->InitializeScrollbars(horizontalChars, verticalChars);
	ShowWindow(this->hWindow, SW_SHOW);
	SwitchToThisWindow(this->hWindow, TRUE);
}

void MainWindow::SetTranslateResult(TranslateResult translateResult)
{
	this->translateResult.Free();
	this->translateResult = translateResult;
	this->Maximize();

	this->headerWindow->RenderResult(this->translateResult);
	this->translateResultWindow->RenderResult(this->translateResult);
}

void MainWindow::InitializeScrollbars(UINT horizontalChars, UINT verticalChars)
{
	SCROLLINFO si;

	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = 0;
	si.nPos = 0;
	si.nMax = verticalChars;
	si.nPage = (int)(WINDOW_HEIGHT / SCROLL_CHAR_Y);
	SetScrollInfo(this->hWindow, SB_VERT, &si, TRUE);

	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = 0;
	si.nPos = 0;
	si.nMax = horizontalChars;
	si.nPage = (int)(WINDOW_WIDTH / SCROLL_CHAR_X);
	SetScrollInfo(this->hWindow, SB_HORZ, &si, TRUE);
}
void MainWindow::ProcessVerticalScroll(WPARAM wParam, LPARAM lParam)
{
	SCROLLINFO si;
	si.cbSize = sizeof (si);
	si.fMask = SIF_ALL;
	
	GetScrollInfo(this->hWindow, SB_VERT, &si);
	int scrollOffsetY = si.nPos;
	
	switch (LOWORD(wParam))
	{
		case SB_TOP:
			si.nPos = si.nMin;
			break;

		case SB_BOTTOM:
			si.nPos = si.nMax;
			break;

		case SB_LINEUP:
			si.nPos -= 1;
			break;

		case SB_LINEDOWN:
			si.nPos += 1;
			break;

		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;

		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;

		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;

		default:
			break;
	}

	si.fMask = SIF_POS;
	SetScrollInfo(this->hWindow, SB_VERT, &si, TRUE);
	GetScrollInfo(this->hWindow, SB_VERT, &si);

	if (si.nPos != scrollOffsetY)
	{
		ScrollWindowEx(
			this->hWindow,
			0,
			int(SCROLL_CHAR_Y * (scrollOffsetY - si.nPos)),
			NULL,
			NULL,
			NULL,
			NULL,
			SW_SCROLLCHILDREN);
	}
}
void MainWindow::ProcessHorizontalScroll(WPARAM wParam, LPARAM lParam)
{
	SCROLLINFO si;
	si.cbSize = sizeof (si);
	si.fMask = SIF_ALL;

	GetScrollInfo(this->hWindow, SB_HORZ, &si);
	int scrollOffsetX = si.nPos;
	switch (LOWORD(wParam))
	{
		case SB_LINELEFT:
			si.nPos -= 1;
			break;

		case SB_LINERIGHT:
			si.nPos += 1;
			break;

		case SB_LEFT:
		case SB_PAGELEFT:
			si.nPos -= si.nPage;
			break;

		case SB_RIGHT:
		case SB_PAGERIGHT:
			si.nPos += si.nPage;
			break;

		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;

		default:
			break;
	}

	si.fMask = SIF_POS;
	SetScrollInfo(this->hWindow, SB_HORZ, &si, TRUE);
	GetScrollInfo(this->hWindow, SB_HORZ, &si);

	if (si.nPos != scrollOffsetX)
	{
		ScrollWindowEx(
			this->hWindow, 
			int(SCROLL_CHAR_X * (scrollOffsetX - si.nPos)),
			0, 
			NULL,
			NULL,
			NULL, 
			NULL, 
			SW_SCROLLCHILDREN);
	}
}
UINT MainWindow::ProcessSizing(WPARAM wParam, LPARAM lParam)
{
	RECT* sizingBox;
	RECT workarea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);
	sizingBox = (RECT*)lParam;
	sizingBox->top = workarea.bottom - WINDOW_HEIGHT - WINDOW_PADDING;
	sizingBox->left = workarea.right - WINDOW_WIDTH - WINDOW_PADDING;
	sizingBox->bottom = sizingBox->top + WINDOW_HEIGHT;
	sizingBox->right = sizingBox->left + WINDOW_WIDTH;
	return TRUE;
}

MainWindow::~MainWindow()
{	
}