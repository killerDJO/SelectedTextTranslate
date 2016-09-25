#include "Windows\MainWindow.h"
#include "Windows\Content\Dictionary\DictionaryWindow.h"
#include "Windows\Content\Translation\TranslationWindow.h"

UINT MainWindow::WM_TASKBARCREATED;

MainWindow::MainWindow(WindowContext* context, WindowDescriptor descriptor, AppModel* appModel)
    : Window(context, descriptor),
    menu(nullptr)
{
    this->appModel = appModel;
    this->className = L"STT_MAIN";
}

void MainWindow::Initialize()
{
    Window::Initialize();

    hWindow = CreateWindowEx(
        WS_EX_TOOLWINDOW,
        className,
        nullptr,
        WS_SIZEBOX | WS_POPUP | WS_CLIPCHILDREN | GetScrollStyle(),
        descriptor.Position.X,
        descriptor.Position.Y,
        descriptor.WindowSize.Width,
        descriptor.WindowSize.Height,
        nullptr,
        nullptr,
        context->GetInstance(),
        this);

    InitNotifyIconData();

    RegisterHotKey(hWindow, ID_TRANSLATE_HOTKEY, MOD_CONTROL, 0x54/*T*/);
    RegisterHotKey(hWindow, ID_PLAYTEXT_HOTKEY, MOD_CONTROL, 0x52/*R*/);

    WM_TASKBARCREATED = RegisterWindowMessageA("TaskbarCreated");

    Minimize();
}

void MainWindow::SpecifyWindowClass(WNDCLASSEX* windowClass)
{
    windowClass->lpfnWndProc = WndProc;
    windowClass->hIcon = LoadIcon(context->GetInstance(), MAKEINTRESOURCE(IDI_APP_ICON));
    windowClass->hIconSm = LoadIcon(context->GetInstance(), MAKEINTRESOURCE(IDI_APP_ICON));
    windowClass->hCursor = LoadCursor(nullptr, IDC_ARROW);
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
    notifyIconData.hIcon = LoadIcon(context->GetInstance(), MAKEINTRESOURCE(IDI_APP_ICON));
    wcscpy_s(notifyIconData.szTip, TEXT("Selected text translate.."));
}

void MainWindow::Minimize()
{
    Shell_NotifyIcon(NIM_ADD, &notifyIconData);
    Hide();
}

void MainWindow::Maximize() const
{
    Show();
    SwitchToThisWindow(hWindow, TRUE);
}

Size MainWindow::RenderDC(Renderer* renderer)
{
    renderer->ClearDC(windowSize);

    Window* renderedView;
    WindowDescriptor dictionaryWindowDescriptor = WindowDescriptor::CreateWindowDescriptor(
        Point(0, 0),
        Size(windowSize.Width, 0),
        OverflowModes::Stretch,
        OverflowModes::Stretch);

    renderedView = appModel->GetCurrentApplicationView() == ApplicactionViews::TranslateResult
        ? (Window*)new TranslationWindow(context, dictionaryWindowDescriptor, hWindow, appModel)
        : (Window*)new DictionaryWindow(context, dictionaryWindowDescriptor, hWindow, appModel);

    AddChildWindow(renderedView);

    renderedView->Render();

    Size contentSize;
    contentSize.Width = renderedView->GetWidth();
    contentSize.Height = renderedView->GetHeight();

    return contentSize;
}

void MainWindow::Scale(double scaleFactorAjustment)
{
    int scaledWidth = context->GetScaleProvider()->Rescale(descriptor.WindowSize.Width, scaleFactorAjustment);
    int scaledHeight = context->GetScaleProvider()->Rescale(descriptor.WindowSize.Height, scaleFactorAjustment);

    descriptor.Position.X -= scaledWidth - descriptor.WindowSize.Width;
    descriptor.Position.Y -= scaledHeight - descriptor.WindowSize.Height;
    position.X = descriptor.Position.X;
    position.Y = descriptor.Position.Y;

    windowSize.Width = descriptor.WindowSize.Width = scaledWidth;
    windowSize.Height = descriptor.WindowSize.Height = scaledHeight;

    context->GetScaleProvider()->AjustScaleFactor(scaleFactorAjustment);

    Render();

    MoveWindow(hWindow, descriptor.Position.X, descriptor.Position.Y, windowSize.Width, windowSize.Height, FALSE);
    SendMessage(hWindow, WM_NCPAINT, NULL, NULL);
}

void MainWindow::Resize()
{
    RECT windowRect;
    GetWindowRect(hWindow, &windowRect);
    descriptor.WindowSize.Width = windowSize.Width = windowRect.right - windowRect.left;
    descriptor.WindowSize.Height = windowSize.Height = windowRect.bottom - windowRect.top;
    position.X = descriptor.Position.X = windowRect.left;
    position.Y = descriptor.Position.Y = windowRect.top;

    context->GetDeviceContextProvider()->ResizeDC(inMemoryDC, windowSize);

    Renderer* renderer = context->GetRenderingContext()->GetRenderer(inMemoryDC);
    renderer->ClearDC(windowSize);
    context->GetRenderingContext()->ReleaseRenderer(renderer);

    context->GetScrollProvider()->ResetScrollPosition(this);

    if (descriptor.OverflowX == OverflowModes::Scroll)
    {
        context->GetScrollProvider()->InitializeScrollbar(this, contentSize.Width, ScrollBars::Horizontal);
    }

    if (descriptor.OverflowY == OverflowModes::Scroll)
    {
        context->GetScrollProvider()->InitializeScrollbar(this, contentSize.Height, ScrollBars::Vertical);
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
        AppendMenu(instance->menu, MF_SEPARATOR, NULL, nullptr);
        AppendMenu(instance->menu, MF_STRING, ID_TRAY_EXIT_CONTEXT_MENU_ITEM, TEXT("Exit"));

        break;
    }

    case WM_SIZE:
        instance->Resize();
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
            UINT clicked = TrackPopupMenu(instance->menu, TPM_RETURNCMD | TPM_NONOTIFY, curPoint.x, curPoint.y, 0, hWnd, nullptr);
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
        if (wParam == ID_ZOOM_IN_HOTKEY)
        {
            instance->Scale(0.1);
        }
        if (wParam == ID_ZOOM_OUT_HOTKEY)
        {
            instance->Scale(-0.1);
        }
        return Window::WndProc(hWnd, message, wParam, lParam);

    case WM_KILLFOCUS:
    {
        HWND windowWithFocus = GetParent((HWND)wParam);
        HWND currentWindow = instance->GetHandle();
        if (windowWithFocus != currentWindow)
        {
            instance->Minimize();
        }
        break;
    }

    case WM_DESTROY:
    case WM_CLOSE:
        instance->appModel->Exit();
        break;

    case WM_SHOWWINDOW:
        if (wParam == TRUE)
        {
            RegisterHotKey(hWnd, ID_ZOOM_IN_HOTKEY, MOD_CONTROL, 0x6B/*Numpad plus*/);
            RegisterHotKey(hWnd, ID_ZOOM_OUT_HOTKEY, MOD_CONTROL, 0x6d/*Numpad minus*/);
        }
        else
        {
            UnregisterHotKey(hWnd, ID_ZOOM_IN_HOTKEY);
            UnregisterHotKey(hWnd, ID_ZOOM_OUT_HOTKEY);
        }
        break;

    default:
        return Window::WndProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

MainWindow::~MainWindow()
{
    Shell_NotifyIcon(NIM_DELETE, &notifyIconData);
}