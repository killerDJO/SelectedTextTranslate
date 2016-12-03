#include "View\Content\MainWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "Utilities\StringUtilities.h"

MainWindow::MainWindow(WindowContext* context)
    : Window(context)
{
    this->className = L"STT_MAIN";
    this->currentView = ApplicationViews::Hidden;

    this->dictionaryWindow = nullptr;
    this->translationWindow = nullptr;
    this->settingsWindow = nullptr;

    this->OnPlayText = Subscribeable<>();
    this->OnForceTranslation = Subscribeable<>();
    this->OnTranslateSuggestion = Subscribeable<>();
    this->OnExpandTranslationResult = Subscribeable<int>();
    this->OnShowTranslation = Subscribeable<int>();
    this->OnSettingsStateChanged = Subscribeable<>();
    this->OnSaveSettings = Subscribeable<Settings>();
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
        nullptr);
    AssertCriticalWinApiResult(windowHandle);
    SetWindowLongPtr(windowHandle, GWLP_USERDATA, (LONG_PTR)this);

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

    translationWindow = new TranslationWindow(context, this);
    translationWindow->SetDescriptor(windowDescriptor);
    AddChildWindow(translationWindow);
    translationWindow->OnPlayText.Subscribe(&OnPlayText);
    translationWindow->OnForceTranslation.Subscribe(&OnForceTranslation);
    translationWindow->OnExpandTranslationResult.Subscribe(&OnExpandTranslationResult);
    translationWindow->OnTranslateSuggestion.Subscribe(&OnTranslateSuggestion);
    translationWindow->MakeHidden();
    translationWindow->SetModel(translateResult);

    dictionaryWindow = new DictionaryWindow(context, this);
    dictionaryWindow->SetDescriptor(windowDescriptor);
    dictionaryWindow->OnShowTranslation.Subscribe(&OnShowTranslation);
    AddChildWindow(dictionaryWindow);
    dictionaryWindow->MakeHidden();
    dictionaryWindow->SetModel(dictionaryRecords);

    settingsWindow = new SettingsWindow(context, this);
    settingsWindow->SetDescriptor(windowDescriptor);
    settingsWindow->OnSettingsStateChanged.Subscribe(&OnSettingsStateChanged);
    settingsWindow->OnSaveSettings.Subscribe(&OnSaveSettings);
    AddChildWindow(settingsWindow);
    settingsWindow->MakeHidden();
    settingsWindow->SetModel(settings);
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
    Hide();
}

void MainWindow::Maximize()
{
    Show();
    SwitchToThisWindow(windowHandle, TRUE);
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

void MainWindow::SetSettingsModel(Settings settings)
{
    AssertWindowInitialized();

    this->settings = settings;
    this->settingsWindow->SetModel(settings);
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
        activeChildWindows[i]->MakeHidden();
    }

    Window* currentView = GetWindowToShow();

    currentView->MakeVisible();
    currentView->Render();

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

    if (currentView == ApplicationViews::Settings)
    {
        return settingsWindow;
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
                default:
                    break;
            }

            return DefWindowProc(windowHandle, message, wParam, lParam);
        }

        case WM_HOTKEY:
            context->GetHotkeyProvider()->ProcessHotkey(wParam);
            return Window::WindowProcedure(message, wParam, lParam);

        case WM_ACTIVATE:
        {
            if(wParam == WA_INACTIVE)
            {
                Minimize();
            }
            return TRUE;
        }

        case WM_SHOWWINDOW:
        {
            if (wParam == TRUE)
            {
                context->GetHotkeyProvider()->RegisterZoomInHotkey(
                    GetHandle(),
                    [=]() -> void { Scale(0.1); });

                context->GetHotkeyProvider()->RegisterZoomOutHotkey(
                    GetHandle(),
                    [=]() -> void { Scale(-0.1); });
            }
            else
            {
                context->GetHotkeyProvider()->UnregisterZoomInHotkey(GetHandle());
                context->GetHotkeyProvider()->UnregisterZoomOutHotkey(GetHandle());
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