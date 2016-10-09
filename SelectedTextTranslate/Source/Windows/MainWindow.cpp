#include "Windows\MainWindow.h"

MainWindow::MainWindow(WindowContext* context, WindowDescriptor descriptor, AppModel* appModel, HotkeyProvider* hotkeyProvider, TrayIconProvider* trayIconProvider)
    : Window(context, descriptor)
{
    this->appModel = appModel;
    this->hotkeyProvider = hotkeyProvider;
    this->trayIconProvider = trayIconProvider;

    this->className = L"STT_MAIN";

    this->dictionaryWindow = nullptr;
    this->translationWindow = nullptr;
}

void MainWindow::Initialize()
{
    Window::Initialize();

    windowHandle = CreateWindowEx(
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

    hotkeyProvider->RegisterTranslateHotkey(windowHandle, [&]() -> void { appModel->TranslateSelectedText(); });
    hotkeyProvider->RegisterPlayTextHotkey(windowHandle, [&]() -> void { appModel->PlaySelectedText(); });

    notifyIconData = trayIconProvider->CreateTrayIcon(context->GetInstance(), windowHandle);

    CreateViews();

    Minimize();
}

void MainWindow::CreateViews()
{
    DestroyChildWindows();

    Size clientSize = GetAvailableClientSize();
    WindowDescriptor windowDescriptor = WindowDescriptor::CreateWindowDescriptor(
        Point(0, 0),
        clientSize,
        OverflowModes::Stretch,
        OverflowModes::Stretch,
        false);

    translationWindow = new TranslationWindow(context, windowDescriptor, this, appModel);
    dictionaryWindow = new DictionaryWindow(context, windowDescriptor, this, appModel);

    translationWindow->Hide();
    dictionaryWindow->Hide();

    AddChildWindow(translationWindow);
    AddChildWindow(dictionaryWindow);
}

void MainWindow::SpecifyWindowClass(WNDCLASSEX* windowClass)
{
    windowClass->lpfnWndProc = WndProc;
    windowClass->hIcon = LoadIcon(context->GetInstance(), MAKEINTRESOURCE(IDI_APP_ICON));
    windowClass->hIconSm = LoadIcon(context->GetInstance(), MAKEINTRESOURCE(IDI_APP_ICON));
    windowClass->hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowClass->hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
}

void MainWindow::Minimize()
{
    ShowWindow(windowHandle, SW_HIDE);
    Hide();
}

void MainWindow::Maximize()
{
    ShowWindow(windowHandle, SW_SHOW);
    SwitchToThisWindow(windowHandle, TRUE);
    Show();
}

Size MainWindow::RenderContent(Renderer* renderer)
{
    ApplicactionViews currentView = appModel->GetCurrentApplicationView();

    for(size_t i = 0; i < activeChildWindows.size(); ++i)
    {
        activeChildWindows[i]->Hide();
    }

    Window* currentWindow;
    if (currentView == ApplicactionViews::TranslateResult)
    {
        currentWindow = translationWindow;
    }
    else
    {
        currentWindow = dictionaryWindow;
    }

    currentWindow->Render();
    currentWindow->Show();

    Size viewContentSize = currentWindow->GetContentSize();
    Size clientSize = GetAvailableClientSize();

    ScrollProvider* scrollProvider = context->GetScrollProvider();
    contentSize.Width = max(viewContentSize.Width, clientSize.Width - scrollProvider->GetVerticalScrollBarWidth());
    contentSize.Height = max(viewContentSize.Height , clientSize.Height - scrollProvider->GetHorizontalScrollBarHeight());

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

    context->GetScaleProvider()->AdjustScaleFactor(scaleFactorAjustment);

    deviceContextBuffer->Resize(windowSize);
    Renderer* renderer = context->GetRenderingContext()->GetRenderer();
    renderer->Render(deviceContextBuffer);
    context->GetRenderingContext()->ReleaseRenderer(renderer);

    MoveWindow(windowHandle, descriptor.Position.X, descriptor.Position.Y, windowSize.Width, windowSize.Height, FALSE);
    SendMessage(windowHandle, WM_NCPAINT, NULL, NULL);

    CreateViews();
    Render();
}

void MainWindow::Resize()
{
    RECT windowRect;
    GetWindowRect(windowHandle, &windowRect);
    int newWidth = windowRect.right - windowRect.left;
    int newHeight = windowRect.bottom - windowRect.top;

    if(descriptor.WindowSize.Width == newWidth && descriptor.WindowSize.Height == newHeight)
    {
        return;
    }

    descriptor.WindowSize.Width = windowSize.Width = newWidth;
    descriptor.WindowSize.Height = windowSize.Height = newHeight;
    position.X = descriptor.Position.X = windowRect.left;
    position.Y = descriptor.Position.Y = windowRect.top;

    deviceContextBuffer->Resize(windowSize);
    Renderer* renderer = context->GetRenderingContext()->GetRenderer();
    renderer->Render(deviceContextBuffer);
    context->GetRenderingContext()->ReleaseRenderer(renderer);

    for (size_t i = 0; i < activeChildWindows.size(); ++i)
    {
        activeChildWindows[i]->Resize();
    }

    Window* currentWindow;
    if (appModel->GetCurrentApplicationView() == ApplicactionViews::TranslateResult)
    {
        currentWindow = translationWindow;
    }
    else
    {
        currentWindow = dictionaryWindow;
    }
    Size viewContentSize = currentWindow->GetContentSize();
    Size clientSize = GetAvailableClientSize();

    ScrollProvider* scrollProvider = context->GetScrollProvider();
    contentSize.Width = max(viewContentSize.Width, clientSize.Width - scrollProvider->GetVerticalScrollBarWidth());
    contentSize.Height = max(viewContentSize.Height, clientSize.Height - scrollProvider->GetHorizontalScrollBarHeight());

    context->GetScrollProvider()->ResetScrollsPosition(this);
    context->GetScrollProvider()->InitializeScrollbars(
        this,
        descriptor.OverflowX == OverflowModes::Scroll,
        descriptor.OverflowY == OverflowModes::Scroll);
}

LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    MainWindow* instance = (MainWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    if(instance != nullptr)
    {
        instance->trayIconProvider->ProcessTrayIconMessages(instance->notifyIconData, hWnd, message, wParam, lParam);
    }

    switch (message)
    {

    case WM_SIZE:
        instance->Resize();
        return TRUE;

    case WM_SYSCOMMAND:
    {
        switch (wParam & 0xfff0)
        {
        case SC_MINIMIZE:
        case SC_CLOSE:
            instance->Minimize();
            return 0;
        }

        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    case WM_HOTKEY:
        instance->hotkeyProvider->ProcessHotkey(wParam);
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
    {
        if (wParam == TRUE)
        {
            instance->hotkeyProvider->RegisterZoomInHotkey(
                instance->GetHandle(),
                [=]() -> void { instance->Scale(0.1); });

            instance->hotkeyProvider->RegisterZoomOutHotkey(
                instance->GetHandle(),
                [=]() -> void { instance->Scale(-0.1); });
        }
        else
        {
            instance->hotkeyProvider->UnregisterZoomInHotkey(instance->GetHandle());
            instance->hotkeyProvider->UnregisterZoomOutHotkey(instance->GetHandle());
        }
        break;
    }

    default:
        return Window::WndProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

MainWindow::~MainWindow()
{
    trayIconProvider->DestroyTrayIcon(notifyIconData);
}