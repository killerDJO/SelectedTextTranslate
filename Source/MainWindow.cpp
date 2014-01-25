#include "stdafx.h"
#include "MainWindow.h"

MainWindow::MainWindow(HINSTANCE hInstance, WNDPROC wndProc)
{
	this->hInstance = hInstance;

	const TCHAR* className = TEXT("tray icon class");

	WNDCLASSEX wnd = { 0 };
	wnd.hInstance = hInstance;
	wnd.lpszClassName = className;
	wnd.lpfnWndProc = wndProc;
	wnd.cbSize = sizeof (WNDCLASSEX);
	wnd.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SELECTEDTEXTTRANSLATE));
	wnd.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
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
	Minimize();
}

HWND MainWindow::GetHandle()
{
	return this->hWindow;
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

void MainWindow::SetTranslateResult(TranslateResult translateResult)
{
	this->translateResult = translateResult;

	UINT horizontalChars = 0;
	UINT verticalChars = 4;

	for (size_t i = 0; i < translateResult.TranslateCategories.size(); ++i) {
		for (size_t j = 0; j <translateResult.TranslateCategories[i].Entries.size(); ++j)
			horizontalChars = max(horizontalChars, translateResult.TranslateCategories[i].Entries[j].Word.length());
		verticalChars += translateResult.TranslateCategories[i].Entries.size() + 1;
	}

	horizontalChars += 4;
	horizontalChars = max(horizontalChars, translateResult.Sentence.Origin.length() + 3);
	horizontalChars = max(horizontalChars, translateResult.Sentence.Translation.length() + 3);

	this->InitializeScrollbars(horizontalChars, verticalChars);
	ShowWindow(this->hWindow, SW_SHOW);
	SwitchToThisWindow(this->hWindow, TRUE);
}

void MainWindow::InitializeScrollbars(UINT horizontalChars, UINT verticalChars)
{
	SCROLLINFO si;

	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = 0;
	si.nPos = 0;
	si.nMax = verticalChars;
	si.nPage = (int)(WINDOW_HEIGHT / yChar);
	SetScrollInfo(this->hWindow, SB_VERT, &si, TRUE);

	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = 0;
	si.nPos = 0;
	si.nMax = horizontalChars;
	si.nPage = (int)(WINDOW_WIDTH / xChar);
	SetScrollInfo(this->hWindow, SB_HORZ, &si, TRUE);
}
void MainWindow::ProcessVerticalScroll(WPARAM wParam, LPARAM lParam)
{
	SCROLLINFO si;
	si.cbSize = sizeof (si);
	si.fMask = SIF_ALL;
	GetScrollInfo(this->hWindow, SB_VERT, &si);

	this->scrollOffsetY = si.nPos;
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

	if (si.nPos != this->scrollOffsetY)
	{
		ScrollWindow(this->hWindow, 0, int(yChar * (this->scrollOffsetY - si.nPos)), NULL, NULL);
	}
}
void MainWindow::ProcessHorizontalScroll(WPARAM wParam, LPARAM lParam)
{
	SCROLLINFO si;
	si.cbSize = sizeof (si);
	si.fMask = SIF_ALL;

	GetScrollInfo(this->hWindow, SB_HORZ, &si);
	this->scrollOffsetX = si.nPos;
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

	if (si.nPos != this->scrollOffsetY)
	{
		ScrollWindow(this->hWindow, int(xChar * (this->scrollOffsetY - si.nPos)), 0, NULL, NULL);
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

void MainWindow::WriteToBuffer(TCHAR* dest, string src)
{
	TCHAR buffer[65535];
	int len = MultiByteToWideChar(20866, 0, src.c_str(), -1, buffer, 65535);

	dest[src.size()] = len;
	std::copy(buffer, buffer + len, dest);
}

void MainWindow::PrintText(HDC hdc, string text, HFONT font, int x, int y)
{
	SelectObject(hdc, font);
	WriteToBuffer(this->buffer, text);
	TextOut(hdc, x, y, this->buffer, _tcslen(this->buffer));
}

void MainWindow::DrawWindow()
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(this->hWindow, &ps);

	RECT rect;

	int scrollX = int(this->scrollOffsetX * this->xChar * -1);
	int scrollY = int(this->scrollOffsetY * this->yChar * -1);

	const int padding = 15, lineHeight = 20;
	const int categoryMargin = 10;
	int curY = padding / 2;

	long lfHeight = -MulDiv(lineHeight / 2, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	long lfHeightHeader = -MulDiv(int(lineHeight * 3 / 5.0), GetDeviceCaps(hdc, LOGPIXELSY), 72);
	HFONT fontNormal = CreateFont(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
	HFONT fontHeader = CreateFont(lfHeightHeader, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
	HFONT fontItalic = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));

	HBRUSH ratingBrush = CreateSolidBrush(RGB(170, 170, 170));

	PrintText(hdc, this->translateResult.Sentence.Translation, fontHeader, padding + scrollX, curY + scrollY);

	rect.right = 0;
	rect.left = WINDOW_WIDTH;
	rect.top = curY + int(5 / 4.0*lineHeight) + scrollY;
	rect.bottom = rect.top + 1;
	FillRect(hdc, &rect, ratingBrush);

	curY += lineHeight + categoryMargin;

	auto categories = this->translateResult.TranslateCategories;
	if (categories.size() == 0)
	{
		SetTextColor(hdc, RGB(119, 119, 119));
		PrintText(hdc, this->translateResult.Sentence.Origin, fontNormal, padding + scrollX, curY + scrollY);
		curY += lineHeight;
	}
	for (size_t i = 0; i < categories.size(); ++i)
	{
		// Draw category header
		SIZE textSize;
		WriteToBuffer(buffer, this->translateResult.Sentence.Origin);
		SelectObject(hdc, fontNormal);
		GetTextExtentPoint32(hdc, buffer, _tcslen(buffer), &textSize);
		TextOut(hdc, padding + scrollX, curY + scrollY, buffer, _tcslen(buffer));

		SetTextColor(hdc, RGB(119, 119, 119));
		PrintText(hdc, " - " + categories[i].PartOfSpeech, fontItalic, padding + textSize.cx + scrollX + 2, curY + scrollY);

		// Draw words
		curY += lineHeight;
		SetTextColor(hdc, RGB(0, 0, 0));
		auto words = categories[i].Entries;
		for (size_t j = 0; j < words.size(); ++j)
		{
			PrintText(hdc, words[j].Word, fontNormal, padding * 3 + scrollX, curY + scrollY);

			int k = words[j].Score >= 0.05 ? 0 : (words[j].Score >= 0.003 ? 1 : 2);
			int rateUnit = 8;

			RECT rect;
			rect.right = padding + rateUnit * 3 + scrollX;
			rect.top = curY + lineHeight / 3 + scrollY;
			rect.bottom = rect.top + lineHeight / 3;
			rect.left = padding + k * rateUnit + scrollX;

			FillRect(hdc, &rect, ratingBrush);

			curY += lineHeight;
		}

		curY += categoryMargin;
	}

	EndPaint(this->hWindow, &ps);
}

MainWindow::~MainWindow()
{	
}