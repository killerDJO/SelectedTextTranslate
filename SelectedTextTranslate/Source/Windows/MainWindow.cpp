#include "Windows\MainWindow.h"
#include "Windows\Content\TranslateResultWindow.h"

UINT MainWindow::WM_TASKBARCREATED;

MainWindow::MainWindow(HINSTANCE hInstance, AppModel* appModel)
{
	this->hInstance = hInstance;
	this->appModel = appModel;

	const TCHAR* className = TEXT("STT_MAIN");

	WNDCLASSEX wnd = { 0 };
	wnd.hInstance = hInstance;
	wnd.lpszClassName = className;
	wnd.lpfnWndProc = WndProc;
	wnd.cbSize = sizeof (WNDCLASSEX);
	wnd.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
	wnd.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
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
		this);

	this->InitNotifyIconData();

	this->dictionaryWindow = new DictionaryWindow(appModel, this->hWindow, this->hInstance, 0, 0, 1000, 5000);
	this->translateResultWindow = new TranslateResultWindow(appModel, this->hWindow, this->hInstance, 0, roundToInt(50 * kY - 2), 2000, 3000);
	this->headerWindow = new HeaderWindow(appModel, this->hWindow, this->hInstance, 0, 0, 2000, roundToInt(50 * kY));

	this->dictionaryWindow->Initialize();
	this->translateResultWindow->Initialize();
	this->headerWindow->Initialize();

	RegisterHotKey(hWindow, ID_TRANSLATE_HOTKEY, MOD_CONTROL, 0x54/*T*/);
	RegisterHotKey(hWindow, ID_PLAYTEXT_HOTKEY, MOD_CONTROL, 0x52/*R*/);

	MainWindow::WM_TASKBARCREATED = RegisterWindowMessageA("TaskbarCreated");

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
	this->notifyIconData.hIcon = LoadIcon(this->hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
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

void MainWindow::ShowTranslateResultView(bool preserveScroll)
{
	int vScroll = 0;

	if (preserveScroll)
	{
		SCROLLINFO si;
		si.cbSize = sizeof (si);
		si.fMask = SIF_POS;
		GetScrollInfo(this->hWindow, SB_VERT, &si);
		vScroll = si.nPos;
	}

	InvalidateRect(this->hWindow, NULL, TRUE);

	this->headerWindow->Show();
	this->translateResultWindow->Show();
	this->dictionaryWindow->Hide();

	POINT headerBottomRight = this->headerWindow->RenderResult();
	POINT contentBottomRight = this->translateResultWindow->RenderResult();

	this->InitializeScrollbars(max(headerBottomRight.x, contentBottomRight.x), headerBottomRight.y + contentBottomRight.y);

	for (int i = 0; i < vScroll; ++i)
	{
		SendMessage(this->hWindow, WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), 0);
	}
}

void MainWindow::ShowDictionaryView()
{
	InvalidateRect(this->hWindow, NULL, TRUE);

	this->headerWindow->Hide();
	this->translateResultWindow->Hide();
	this->dictionaryWindow->Show();

	POINT contentBottomRight = this->dictionaryWindow->RenderResult();

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

LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MainWindow* instance = (MainWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if (message == MainWindow::WM_TASKBARCREATED && !IsWindowVisible(instance->hWindow))
	{
		instance->Minimize();
		return 0;
	}

	HWND windowWithFocus, currentWindow;

	int zDelta;
	switch (message)
	{

	case WM_CREATE:
	{
		CREATESTRUCT* createstruct = (CREATESTRUCT*)lParam;
		instance = (MainWindow*)createstruct->lpCreateParams;
		instance->hWindow = hWnd;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)createstruct->lpCreateParams);

		instance->menu = CreatePopupMenu();
		AppendMenu(instance->menu, MF_STRING, ID_TRAY_TRANSLATE_CONTEXT_MENU_ITEM, TEXT("Translate from clipboard"));
		AppendMenu(instance->menu, MF_STRING, ID_TRAY_DICTIONARY_CONTEXT_MENU_ITEM, TEXT("Dictionary"));
		AppendMenu(instance->menu, MF_SEPARATOR, NULL, NULL);
		AppendMenu(instance->menu, MF_STRING, ID_TRAY_EXIT_CONTEXT_MENU_ITEM, TEXT("Exit"));

		break;
	}

	case WM_SIZING:
		return instance->ProcessSizing(wParam, lParam);

	case WM_SETCURSOR:
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		return TRUE;

	case WM_HSCROLL:
		instance->ProcessHorizontalScroll(wParam, lParam);
		break;

	case WM_VSCROLL:
		instance->ProcessVerticalScroll(wParam, lParam);
		break;

	case WM_MOUSEWHEEL:
		zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (zDelta < 0)
			SendMessage(instance->GetHandle(), WM_VSCROLL, SB_LINEDOWN, NULL);
		else
			SendMessage(instance->GetHandle(), WM_VSCROLL, SB_LINEUP, NULL);

	case WM_SYSCOMMAND:
		switch (wParam & 0xfff0)
		{
			case SC_MINIMIZE:
			case SC_CLOSE:
				instance->Minimize();
				return 0;
		}

		return DefWindowProc(hWnd, message, wParam, lParam);

	case WM_TRAYICON:
		
		if (lParam == WM_LBUTTONUP)
		{
			instance->appModel->TranslateSelectedText();
		}

		if (lParam == WM_RBUTTONUP)
		{
			POINT curPoint;
			GetCursorPos(&curPoint);
			SetForegroundWindow(hWnd);
			UINT clicked = TrackPopupMenu(instance->menu, TPM_RETURNCMD | TPM_NONOTIFY, curPoint.x, curPoint.y, 0, hWnd, NULL);
			if (clicked == ID_TRAY_EXIT_CONTEXT_MENU_ITEM)
			{
				instance->appModel->Exit();
			}
			if (clicked == ID_TRAY_TRANSLATE_CONTEXT_MENU_ITEM)
			{
				instance->appModel->TranslateSelectedText();
			}
			if (clicked == ID_TRAY_DICTIONARY_CONTEXT_MENU_ITEM)
			{
				instance->appModel->ShowDictionary();
			}
		}
		break;

	case WM_HOTKEY:
		if (wParam == ID_TRANSLATE_HOTKEY)
		{
			instance->appModel->TranslateSelectedText();
		}
		if (wParam == ID_PLAYTEXT_HOTKEY)
		{
			instance->appModel->PlaySelectedText();
		}
		break;

	case WM_KILLFOCUS:
		windowWithFocus = GetParent((HWND)wParam);
		currentWindow = instance->GetHandle();
		if (windowWithFocus != currentWindow)
		{
			instance->Minimize();
		}
		break;

	case WM_DESTROY:
	case WM_CLOSE:
		instance->appModel->Exit();
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

MainWindow::~MainWindow()
{	
	delete this->headerWindow;
	delete this->translateResultWindow;
	delete this->dictionaryWindow;
}