#include "Windows\MainWindow.h"

UINT MainWindow::WM_TASKBARCREATED;

MainWindow::MainWindow(HINSTANCE hInstance, AppModel* appModel, RenderingContext* renderingContext, ScrollProvider* scrollProvider, WindowDescriptor descriptor)
    : Window(hInstance, renderingContext, scrollProvider, descriptor)
{
    this->renderingContext = renderingContext;
    this->scrollProvider = scrollProvider;
    this->appModel = appModel;
    this->className = L"STT_MAIN";
}

void MainWindow::Initialize()
{
    Window::Initialize();

    hWindow = CreateWindowEx(
        WS_EX_TOOLWINDOW,
        className,
        NULL,
        WS_SIZEBOX | WS_POPUP | WS_CLIPCHILDREN | GetScrollStyle(),
        descriptor.X,
        descriptor.Y,
        descriptor.Width,
        descriptor.Height,
        NULL,
        NULL,
        hInstance,
        this);

    InitNotifyIconData();

    int headerHeight = 50;

    WindowDescriptor dictionaryWindowDescriptor = WindowDescriptor::CreateStretchWindowDescriptor(0, 0);
    dictionaryWindow = new DictionaryWindow(hInstance, renderingContext, scrollProvider, renderingContext->Scale(dictionaryWindowDescriptor), hWindow, appModel);

    WindowDescriptor translateResultWindowDescriptor = WindowDescriptor::CreateWindowDescriptor(0, headerHeight - 1, descriptor.Width, 0, OverflowModes::Stretch, OverflowModes::Stretch);
    translateResultWindow = new TranslateResultWindow(hInstance, renderingContext, scrollProvider, renderingContext->Scale(translateResultWindowDescriptor), hWindow, appModel);

    WindowDescriptor headerWindowDescriptor = WindowDescriptor::CreateWindowDescriptor(0, 0, descriptor.Width, headerHeight, OverflowModes::Stretch, OverflowModes::Fixed);
    headerWindow = new HeaderWindow(hInstance, renderingContext, scrollProvider, renderingContext->Scale(headerWindowDescriptor), hWindow, appModel);

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

SIZE MainWindow::RenderContent()
{
    SIZE renderedSize = appModel->GetCurrentApplicationView() == ApplicactionViews::TranslateResult
        ? RenderTranslateResultView()
        : RenderDictionaryView();

    HDC hdc = GetDC(hWindow);
    renderingContext->ClearDC(hdc, descriptor.Width, descriptor.Height);
    DeleteDC(hdc);

    return renderedSize;
}

SIZE MainWindow::RenderTranslateResultView()
{
    headerWindow->Show();
    translateResultWindow->Show();
    dictionaryWindow->Hide();

    headerWindow->Render();
    translateResultWindow->Render();

    SIZE contentSize;
    contentSize.cx = max(headerWindow->GetWidth(), translateResultWindow->GetWidth());
    contentSize.cy = headerWindow->GetHeight() + translateResultWindow->GetHeight();
    return contentSize;
}

SIZE MainWindow::RenderDictionaryView()
{
    headerWindow->Hide();
    translateResultWindow->Hide();
    dictionaryWindow->Show();

    dictionaryWindow->Render();

    SIZE contentSize;
    contentSize.cx = dictionaryWindow->GetWidth();
    contentSize.cy = dictionaryWindow->GetHeight();
    return contentSize;
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
        return Window::WndProc(hWnd, message, wParam, lParam);
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