#include "View\Content\MainWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "Utilities\StringUtilities.h"

MainWindow::MainWindow(WindowContext* context, WindowDescriptor descriptor, HotkeyProvider* hotkeyProvider)
    : Window(context, descriptor)
{
    this->hotkeyProvider = hotkeyProvider;

    this->className = L"STT_MAIN";
    this->currentView = ApplicationViews::Hidden;

    this->dictionaryWindow = nullptr;
    this->translationWindow = nullptr;

    this->OnPlayText = Subscribeable<>();
    this->OnForceTranslation = Subscribeable<>();
    this->OnTranslateSuggestion = Subscribeable<>();
    this->OnExpandTranslationResult = Subscribeable<int>();
    this->OnShowTranslation = Subscribeable<int>();
}

void MainWindow::Initialize()
{
    Window::Initialize();

    windowHandle = CreateWindowEx(
        WS_EX_TOOLWINDOW,
        className,
        nullptr,
        WS_SIZEBOX | WS_POPUP | WS_CLIPCHILDREN | GetScrollStyle(),
        descriptor.GetPosition().X,
        descriptor.GetPosition().Y,
        descriptor.GetWindowSize().Width,
        descriptor.GetWindowSize().Height,
        nullptr,
        nullptr,
        context->GetInstance(),
        this);
    AssertCriticalWinApiResult(windowHandle);

    CreateViews();

    Minimize();

    context->GetErrorHandler()->OnErrorShow.Subscribe(bind(&MainWindow::Minimize, this));
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

    translationWindow = new TranslationWindow(context, windowDescriptor, this);
    AddChildWindow(translationWindow);
    translationWindow->OnPlayText.Subscribe(&OnPlayText);
    translationWindow->OnForceTranslation.Subscribe(&OnForceTranslation);
    translationWindow->OnExpandTranslationResult.Subscribe(&OnExpandTranslationResult);
    translationWindow->OnTranslateSuggestion.Subscribe(&OnTranslateSuggestion);
    translationWindow->Hide();
    translationWindow->SetModel(translateResult);

    dictionaryWindow = new DictionaryWindow(context, windowDescriptor, this);
    dictionaryWindow->OnShowTranslation.Subscribe(&OnShowTranslation);
    AddChildWindow(dictionaryWindow);
    dictionaryWindow->Hide();
    dictionaryWindow->SetModel(dictionaryRecords);
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
    AssertWindowInitialized();

    ShowWindow(windowHandle, SW_HIDE);
    Hide();
}

void MainWindow::Maximize()
{
    AssertWindowInitialized();

    ShowWindow(windowHandle, SW_SHOW);
    SwitchToThisWindow(windowHandle, TRUE);
    Show();
}

void MainWindow::SetTranslateResultModel(TranslateResult translateResult)
{
    AssertWindowInitialized();

    this->translateResult = translateResult;
    this->translationWindow->SetModel(translateResult);
}

void MainWindow::SetDictionaryModel(vector<DictionaryRecord> dictionaryRecords)
{
    AssertWindowInitialized();

    this->dictionaryRecords = dictionaryRecords;
    this->dictionaryWindow->SetModel(dictionaryRecords);
}

void MainWindow::SetCurrentView(ApplicationViews applicationView)
{
    AssertWindowInitialized();

    currentView = applicationView;
}

Size MainWindow::RenderContent(Renderer* renderer)
{
    for(size_t i = 0; i < activeChildWindows.size(); ++i)
    {
        activeChildWindows[i]->Hide();
    }

    Window* currentView = GetWindowToShow();

    currentView->Render();
    currentView->Show();

    return currentView->GetContentSize();
}

void MainWindow::Scale(double scaleFactorAdjustment)
{
    int scaledWidth = context->GetScaleProvider()->Rescale(descriptor.GetWindowSize().Width, scaleFactorAdjustment);
    int scaledHeight = context->GetScaleProvider()->Rescale(descriptor.GetWindowSize().Height, scaleFactorAdjustment);

    position.X = descriptor.GetPosition().X - scaledWidth + descriptor.GetWindowSize().Width;
    position.Y = descriptor.GetPosition().Y - scaledHeight + descriptor.GetWindowSize().Height;
    descriptor.SetPosition(position);

    windowSize.Width = scaledWidth;
    windowSize.Height =  scaledHeight;
    descriptor.SetWindowSize(windowSize);

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

    if(descriptor.GetWindowSize().Width == newWidth && descriptor.GetWindowSize().Height == newHeight)
    {
        return;
    }

    windowSize.Width = newWidth;
    windowSize.Height = newHeight;
    descriptor.SetWindowSize(windowSize);

    position.X = windowRect.left;
    position.Y = windowRect.top;
    descriptor.SetPosition(position);

    deviceContextBuffer->Resize(windowSize);

    // Clear background
    Renderer* renderer = context->GetRenderingContext()->GetRenderer();
    renderer->Render(deviceContextBuffer);
    context->GetRenderingContext()->ReleaseRenderer(renderer);

    for (size_t i = 0; i < activeChildWindows.size(); ++i)
    {
        activeChildWindows[i]->Resize();
    }

    Window* currentWindow = GetWindowToShow();
    contentSize = currentWindow->GetContentSize();

    ApplyRenderedState(true);
}

Window* MainWindow::GetWindowToShow() const
{
    if (currentView == ApplicationViews::TranslateResult)
    {
        return translationWindow;
    }

    if(currentView == ApplicationViews::Dictionary)
    {
        return dictionaryWindow;
    }

    throw SelectedTextTranslateFatalException(StringUtilities::Format(L"Unsupported view: %d", currentView));
}

LRESULT MainWindow::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
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