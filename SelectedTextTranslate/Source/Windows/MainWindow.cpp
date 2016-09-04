#include "Windows\MainWindow.h"
#include "Windows\Content\TranslateResultWindow.h"

UINT MainWindow::WM_TASKBARCREATED;

MainWindow::MainWindow(HINSTANCE hInstance, AppModel* appModel)
    : Window(hInstance)
{
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
        WS_SIZEBOX | WS_POPUP | WS_HSCROLL | WS_VSCROLL,
        workarea.right - width - padding,
        workarea.bottom - height - padding,
        width,
        height,
        NULL,
        NULL,
        hInstance,
        this);

    InitNotifyIconData();

    dictionaryWindow = new DictionaryWindow(appModel, hWindow, hInstance, 0, 0, 1000, 5000);
    translateResultWindow = new TranslateResultWindow(appModel, hWindow, hInstance, 0, roundToInt(50 * kY - 2), 2000, 3000);
    headerWindow = new HeaderWindow(appModel, hWindow, hInstance, 0, 0, 2000, roundToInt(50 * kY));

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
    UINT screenResolutionX = workarea.right - workarea.left;
    UINT screenResolutionY = workarea.bottom - workarea.top;
    kX = screenResolutionX / 1440.0;
    kY = screenResolutionY / 860.0;

    width = roundToInt(300 * this->kX);
    height = roundToInt(400 * this->kY);
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
    int vScroll = 0;

    if (preserveScroll)
    {
        SCROLLINFO si;
        si.cbSize = sizeof (si);
        si.fMask = SIF_POS;
        GetScrollInfo(hWindow, SB_VERT, &si);
        vScroll = si.nPos;
    }

    headerWindow->Show();
    translateResultWindow->Show();
    dictionaryWindow->Hide();

    POINT headerBottomRight = headerWindow->RenderResult();
    POINT contentBottomRight = translateResultWindow->RenderResult();

    InitializeScrollbars(max(headerBottomRight.x, contentBottomRight.x), headerBottomRight.y + contentBottomRight.y);

    for (int i = 0; i < vScroll; ++i)
    {
        SendMessage(hWindow, WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), 0);
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
    si.nMax = int(contentDimension / scrollChar);
    si.nPage = (int)(windowDimension / scrollChar);
    SetScrollInfo(this->hWindow, nBar, &si, TRUE);
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
    SCROLLINFO si;
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;

    GetScrollInfo(hWindow, nBar, &si);
    int scrollOffset = si.nPos;

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
    SetScrollInfo(hWindow, nBar, &si, TRUE);
    GetScrollInfo(hWindow, nBar, &si);

    if (si.nPos != scrollOffset)
    {
        int scrollAmount = int(scrollChar * (scrollOffset - si.nPos));
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

        //Sleep(100);
    }
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