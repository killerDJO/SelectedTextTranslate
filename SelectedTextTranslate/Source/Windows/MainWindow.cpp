#include "Windows\MainWindow.h"
#include "Windows\Content\TranslateResultWindow.h"

UINT MainWindow::WM_TASKBARCREATED;

MainWindow::MainWindow(HINSTANCE hInstance, AppModel* appModel, Renderer* renderer)
    : Window(hInstance, renderer)
{
    this->renderer = renderer;
    this->appModel = appModel;
    this->className = L"STT_MAIN";

    this->scrollCharX = 8;
    this->scrollCharY = 20;
}

void MainWindow::Initialize()
{
    Window::Initialize();

    RECT workarea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);

    ComputeWindowDimensions(workarea);

    hWindow = CreateWindowEx(
        WS_EX_TOOLWINDOW,
        className,
        NULL,
        WS_SIZEBOX | WS_POPUP | WS_HSCROLL | WS_VSCROLL | WS_CLIPCHILDREN,
        workarea.right - width - padding,
        workarea.bottom - height - padding,
        width,
        height,
        NULL,
        NULL,
        hInstance,
        this);

    InitNotifyIconData();

    int headerHeight = renderer->AdjustToYResolution(50);
    dictionaryWindow = new DictionaryWindow(renderer, appModel, hWindow, hInstance, 0, 0);
    translateResultWindow = new TranslateResultWindow(renderer, appModel, hWindow, hInstance, 0, headerHeight - 1);
    headerWindow = new HeaderWindow(renderer, appModel, hWindow, hInstance, 0, 0, headerHeight);

    dictionaryWindow->Initialize();
    translateResultWindow->Initialize();
    headerWindow->Initialize();

    RegisterHotKey(hWindow, ID_TRANSLATE_HOTKEY, MOD_CONTROL, 0x54/*T*/);
    RegisterHotKey(hWindow, ID_PLAYTEXT_HOTKEY, MOD_CONTROL, 0x52/*R*/);

    MainWindow::WM_TASKBARCREATED = RegisterWindowMessageA("TaskbarCreated");

    Minimize();
}

void MainWindow::SpecifyWindowClass(WNDCLASSEX* windowClass)
{
    windowClass->lpfnWndProc = WndProc;
    windowClass->hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
    windowClass->hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
    windowClass->hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass->hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
}

void MainWindow::ComputeWindowDimensions(RECT workarea)
{
    width = renderer->AdjustToXResolution(300);
    height = renderer->AdjustToXResolution(400);
    padding = 5;
}

void MainWindow::InitNotifyIconData()
{
    memset(&notifyIconData, 0, sizeof(NOTIFYICONDATA));

    notifyIconData.cbSize = sizeof(NOTIFYICONDATA);

    notifyIconData.hWnd = hWindow;
    notifyIconData.uID = ID_TRAY_APP_ICON;
    notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    notifyIconData.uCallbackMessage = WM_TRAYICON;
    notifyIconData.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
    wcscpy_s(notifyIconData.szTip, TEXT("Selected text translate.."));
}

void MainWindow::Minimize()
{
    Shell_NotifyIcon(NIM_ADD, &notifyIconData);
    Hide();
}

void MainWindow::Maximize()
{
    Show();
    SwitchToThisWindow(hWindow, TRUE);
}

void MainWindow::ShowTranslateResultView(bool preserveScroll)
{
    DWORD verticalScroll = 0;

    if (preserveScroll)
    {
        SCROLLINFO scrollInfo = GetScrollBarInfo(SB_VERT);
        verticalScroll = scrollInfo.nPos;
    }

    headerWindow->Show();
    translateResultWindow->Show();
    dictionaryWindow->Hide();

    POINT headerBottomRight = headerWindow->RenderResult();
    POINT contentBottomRight = translateResultWindow->RenderResult();

    InitializeScrollbars(max(headerBottomRight.x, contentBottomRight.x), headerBottomRight.y + contentBottomRight.y);

    if (preserveScroll)
    {
        SCROLLINFO scrollInfo = GetScrollBarInfo(SB_VERT);
        int scrollOffset = scrollInfo.nPos;

        scrollInfo.nPos = min(scrollInfo.nMax - scrollInfo.nPage + 1, verticalScroll);

        SetScrollPosition(scrollInfo, SB_VERT, scrollOffset, scrollCharY);
    }
}

void MainWindow::ShowDictionaryView()
{
    headerWindow->Hide();
    translateResultWindow->Hide();
    dictionaryWindow->Show();

    POINT contentBottomRight = dictionaryWindow->RenderResult();

    InitializeScrollbars(contentBottomRight.x, contentBottomRight.y);

    Maximize();
}

void MainWindow::InitializeScrollbars(int contentWidth, int contentHeight)
{
    InitializeScrollbar(height, contentHeight, scrollCharY, SB_VERT);
    InitializeScrollbar(width, contentWidth, scrollCharX, SB_HORZ);
}

void MainWindow::InitializeScrollbar(int windowDimension, int contentDimension, int scrollChar, int nBar)
{
    SCROLLINFO si;

    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMin = 0;
    si.nPos = 0;
    si.nMax = roundToInt(contentDimension / scrollChar);
    si.nPage = roundToInt(windowDimension / scrollChar);
    SetScrollInfo(hWindow, nBar, &si, TRUE);
}

void MainWindow::ProcessVerticalScroll(WPARAM wParam, LPARAM lParam)
{
    ProcessScroll(wParam, lParam, scrollCharY, SB_VERT);
}

void MainWindow::ProcessHorizontalScroll(WPARAM wParam, LPARAM lParam)
{
    ProcessScroll(wParam, lParam, scrollCharX, SB_HORZ);
}

void MainWindow::ProcessScroll(WPARAM wParam, LPARAM lParam, int scrollChar, int nBar)
{
    SCROLLINFO scrollInfo = GetScrollBarInfo(nBar);
    int scrollOffset = scrollInfo.nPos;

    switch (LOWORD(wParam))
    {
    case SB_TOP:
        scrollInfo.nPos = scrollInfo.nMin;
        break;

    case SB_BOTTOM:
        scrollInfo.nPos = scrollInfo.nMax;
        break;

    case SB_LINEUP:
        scrollInfo.nPos -= 1;
        break;

    case SB_LINEDOWN:
        scrollInfo.nPos += 1;
        break;

    case SB_PAGEUP:
        scrollInfo.nPos -= scrollInfo.nPage;
        break;

    case SB_PAGEDOWN:
        scrollInfo.nPos += scrollInfo.nPage;
        break;

    case SB_THUMBTRACK:
        scrollInfo.nPos = scrollInfo.nTrackPos;
        break;

    default:
        break;
    }

    SetScrollPosition(scrollInfo, nBar, scrollOffset, scrollChar);
}

void MainWindow::SetScrollPosition(SCROLLINFO scrollInfo, int nBar, int scrollOffset, int scrollChar)
{
    scrollInfo.fMask = SIF_POS;
    SetScrollInfo(hWindow, nBar, &scrollInfo, TRUE);
    GetScrollInfo(hWindow, nBar, &scrollInfo);

    if (scrollInfo.nPos != scrollOffset)
    {
        int scrollAmount = roundToInt(scrollChar * (scrollOffset - scrollInfo.nPos));
        int scrollAmountHorizontal = 0;
        int scrollAmountVertical = 0;

        if (nBar == SB_VERT)
        {
            scrollAmountVertical = scrollAmount;
        }
        else
        {
            scrollAmountHorizontal = scrollAmount;
        }

        ScrollWindowEx(
            hWindow,
            scrollAmountHorizontal,
            scrollAmountVertical,
            NULL,
            NULL,
            NULL,
            NULL,
            SW_SCROLLCHILDREN);

        HDC hdc = GetDC(hWindow);
        renderer->ClearDC(hdc, width, height);
        DeleteDC(hdc);
    }
}

SCROLLINFO MainWindow::GetScrollBarInfo(int nBar)
{
    SCROLLINFO scrollInfo;
    scrollInfo.cbSize = sizeof(scrollInfo);
    scrollInfo.fMask = SIF_ALL;
    GetScrollInfo(hWindow, nBar, &scrollInfo);

    return scrollInfo;
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
    Shell_NotifyIcon(NIM_DELETE, &notifyIconData);

    delete headerWindow;
    delete translateResultWindow;
    delete dictionaryWindow;
}