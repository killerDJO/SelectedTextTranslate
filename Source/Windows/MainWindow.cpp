#include "PrecompiledHeaders\stdafx.h"
#include "Windows\MainWindow.h"

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
	
	this->ComputeWindowDimensions(workarea);

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

	this->dictionaryWindow = new DictionaryWindow(this->hWindow, this->hInstance, 0, 0, 2000, 3000);
	this->translateResultWindow = new TranslateResultWindow(this, 0, roundToInt(50 * kY - 2), 2000, 3000);
	this->headerWindow = new HeaderWindow(this->hWindow, this->hInstance, 0, 0, 2000, roundToInt(50 * kY));

	Minimize();
}

void MainWindow::ComputeWindowDimensions(RECT workarea)
{
	UINT screenResolutionX = workarea.right - workarea.left;
	UINT screenResolutionY = workarea.bottom - workarea.top;
	this->kX = screenResolutionX / 1440.0;
	this->kY = screenResolutionY / 860.0;

	this->WINDOW_WIDTH = roundToInt(300 * this->kX);
	this->WINDOW_HEIGHT = roundToInt(400 * this->kY);
	this->WINDOW_PADDING = 5;
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
	ShowWindow(this->hWindow, SW_SHOW);
	SwitchToThisWindow(this->hWindow, TRUE);
}

void MainWindow::PlayText()
{
	this->headerWindow->PlayText();
}

void MainWindow::SetTranslateResult(TranslateResult translateResult, BOOL maximize)
{
	this->translateResult.Free();
	
	this->Render(translateResult);
	
	if (maximize)
	{
		this->Maximize();
	}
}

void MainWindow::Render(TranslateResult translateResult, int vScroll)
{
	InvalidateRect(this->hWindow, NULL, TRUE);

	this->translateResult = translateResult;

	this->headerWindow->Show();
	this->translateResultWindow->Show();
	this->dictionaryWindow->Hide();

	POINT headerBottomRight = this->headerWindow->RenderResult(this->translateResult);
	POINT contentBottomRight = this->translateResultWindow->RenderResult(this->translateResult);

	this->InitializeScrollbars(max(headerBottomRight.x, contentBottomRight.x), headerBottomRight.y + contentBottomRight.y);

	for (int i = 0; i < vScroll; ++i){
		SendMessage(this->hWindow, WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), 0);
	}
}

void MainWindow::RenderDictionary(vector<LogRecord> records)
{
	InvalidateRect(this->hWindow, NULL, TRUE);

	this->headerWindow->Hide();
	this->translateResultWindow->Hide();
	this->dictionaryWindow->Show();

	POINT contentBottomRight = this->dictionaryWindow->RenderResult(records);

	this->InitializeScrollbars(contentBottomRight.x, contentBottomRight.y);

	this->Maximize();
}

void MainWindow::InitializeScrollbars(int contentWidth, int contentHeight)
{
	SCROLLINFO si;

	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = 0;
	si.nPos = 0;
	si.nMax = int(contentHeight / SCROLL_CHAR_Y);
	si.nPage = (int)(WINDOW_HEIGHT / SCROLL_CHAR_Y);
	SetScrollInfo(this->hWindow, SB_VERT, &si, TRUE);

	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = 0;
	si.nPos = 0;
	si.nMax = int(contentWidth / SCROLL_CHAR_X);
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