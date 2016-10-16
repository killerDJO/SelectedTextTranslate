#include "View\Content\MainWindow.h"
#include "Exceptions\SelectedTextTranslateException.h"
#include "Helpers\ExceptionHelper.h"

MainWindow::MainWindow(WindowContext* context, WindowDescriptor descriptor, AppController* appController, HotkeyProvider* hotkeyProvider, TrayIconProvider* trayIconProvider)
    : Window(context, descriptor)
{
    this->appController = appController;
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
    AssertCriticalWinApiResult(windowHandle);

    hotkeyProvider->RegisterTranslateHotkey(windowHandle, [&]() -> void { appController->TranslateSelectedText(); });
    hotkeyProvider->RegisterPlayTextHotkey(windowHandle, [&]() -> void { appController->PlaySelectedText(); });

    trayIconProvider->CreateTrayIcon(windowHandle, context->GetInstance());

    CreateViews();

    Minimize();
}

void MainWindow::CreateViews()
{
    DestroyChildWindows();

    WindowDescriptor windowDescriptor = WindowDescriptor::CreateWindowDescriptor(
        Point(0, 0),
        windowSize,
        OverflowModes::Stretch,
        OverflowModes::Stretch,
        false);

    translationWindow = new TranslationWindow(context, windowDescriptor, this, appController);
    dictionaryWindow = new DictionaryWindow(context, windowDescriptor, this, appController);

    translationWindow->Hide();
    dictionaryWindow->Hide();

    AddChildWindow(translationWindow);
    AddChildWindow(dictionaryWindow);
}

void MainWindow::SpecifyWindowClass(WNDCLASSEX* windowClass)
{
    windowClass->hIcon = LoadIcon(context->GetInstance(), MAKEINTRESOURCE(IDI_APP_ICON));
    AssertCriticalWinApiResult(windowClass->hIcon);

    windowClass->hIconSm = LoadIcon(context->GetInstance(), MAKEINTRESOURCE(IDI_APP_ICON));
    AssertCriticalWinApiResult(windowClass->hIconSm);

    windowClass->hCursor = LoadCursor(nullptr, IDC_ARROW);
    AssertCriticalWinApiResult(windowClass->hCursor);

    windowClass->hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    AssertCriticalWinApiResult(windowClass->hbrBackground);
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

void MainWindow::ShowError(wstring message)
{
    this->Minimize();
    this->trayIconProvider->ShowErrorMessage(message);
}

Size MainWindow::RenderContent(Renderer* renderer)
{
    for(size_t i = 0; i < activeChildWindows.size(); ++i)
    {
        activeChildWindows[i]->Hide();
    }

    Window* currentView = GetCurrentView();

    currentView->Render();
    currentView->Show();

    return currentView->GetContentSize();
}

void MainWindow::Scale(double scaleFactorAdjustment)
{
    int scaledWidth = context->GetScaleProvider()->Rescale(descriptor.WindowSize.Width, scaleFactorAdjustment);
    int scaledHeight = context->GetScaleProvider()->Rescale(descriptor.WindowSize.Height, scaleFactorAdjustment);

    descriptor.Position.X -= scaledWidth - descriptor.WindowSize.Width;
    descriptor.Position.Y -= scaledHeight - descriptor.WindowSize.Height;
    position.X = descriptor.Position.X;
    position.Y = descriptor.Position.Y;

    windowSize.Width = descriptor.WindowSize.Width = scaledWidth;
    windowSize.Height = descriptor.WindowSize.Height = scaledHeight;

    context->GetScaleProvider()->AdjustScaleFactor(scaleFactorAdjustment);

    deviceContextBuffer->Resize(windowSize);

    CreateViews();
    Render();
}

void MainWindow::Resize()
{
    if(windowState == WindowStates::Rendering)
    {
        return;
    }

    RECT windowRect;
    AssertCriticalWinApiResult(GetWindowRect(windowHandle, &windowRect));
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

    // Clear background
    Renderer* renderer = context->GetRenderingContext()->GetRenderer();
    renderer->Render(deviceContextBuffer);
    context->GetRenderingContext()->ReleaseRenderer(renderer);

    for (size_t i = 0; i < activeChildWindows.size(); ++i)
    {
        activeChildWindows[i]->Resize();
    }

    Window* currentView = GetCurrentView();
    contentSize = currentView->GetContentSize();

    ApplyRenderedState(true);
}

Window* MainWindow::GetCurrentView() const
{
    ApplicationViews currentView = appController->GetCurrentApplicationView();
    if (currentView == ApplicationViews::TranslateResult)
    {
        return translationWindow;
    }

    if(currentView == ApplicationViews::Dictionary)
    {
        return dictionaryWindow;
    }

    ThrowSelectedTextTranslateFatalException(StringUtilities::Format(L"Unsupported view: %d", currentView));
}

LRESULT MainWindow::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    this->trayIconProvider->ProcessTrayIconMessages(windowHandle, message, wParam, lParam);

    switch (message)
    {

    case WM_SIZE:
        Resize();
        return TRUE;

    case WM_SYSCOMMAND:
    {
        switch (wParam & 0xfff0)
        {
        case SC_MINIMIZE:
        case SC_CLOSE:
            Minimize();
            return 0;
        }

        return DefWindowProc(windowHandle, message, wParam, lParam);
    }

    case WM_HOTKEY:
        hotkeyProvider->ProcessHotkey(wParam);
        return Window::WindowProcedure(message, wParam, lParam);

    case WM_KILLFOCUS:
    {
        HWND windowWithFocus = GetParent((HWND)wParam);
        HWND currentWindow = GetHandle();
        if (windowWithFocus != currentWindow)
        {
            Minimize();
        }
        break;
    }

    case WM_DESTROY:
    case WM_CLOSE:
        appController->Exit();
        break;

    case WM_SHOWWINDOW:
    {
        if (wParam == TRUE)
        {
            hotkeyProvider->RegisterZoomInHotkey(
                GetHandle(),
                [=]() -> void { Scale(0.1); });

            hotkeyProvider->RegisterZoomOutHotkey(
                GetHandle(),
                [=]() -> void { Scale(-0.1); });
        }
        else
        {
            hotkeyProvider->UnregisterZoomInHotkey(GetHandle());
            hotkeyProvider->UnregisterZoomOutHotkey(GetHandle());
        }
        break;
    }

    default:
        return Window::WindowProcedure(message, wParam, lParam);
    }

    return 0;
}

MainWindow::~MainWindow()
{
}