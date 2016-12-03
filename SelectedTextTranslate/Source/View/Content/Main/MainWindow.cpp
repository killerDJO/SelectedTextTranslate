#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "Utilities\StringUtilities.h"
#include "View\Content\Main\MainWindow.h"
#include "View\Controls\Dialogs\Confirm\ConfirmDialogWindow.h"

MainWindow::MainWindow(WindowContext* context, HotkeyProvider* hotkeyProvider)
    : Window(context)
{
    this->hotkeyProvider = hotkeyProvider;

    this->className = L"STT_MAIN";
    this->currentView = ApplicationViews::None;

    this->dictionaryWindow = nullptr;
    this->translationWindow = nullptr;
    this->settingsWindow = nullptr;
    this->confirmDialogWindow = nullptr;

    this->OnPlayText = Subscribeable<>();
    this->OnForceTranslation = Subscribeable<>();
    this->OnTranslateSuggestion = Subscribeable<>();
    this->OnExpandTranslationResult = Subscribeable<int>();
    this->OnShowTranslation = Subscribeable<int>();
    this->OnSettingsStateChanged = Subscribeable<>();
    this->OnSaveSettings = Subscribeable<Settings>();

    this->viewDescriptors = map<ApplicationViews, ViewDescriptor>();
}

void MainWindow::SetDescriptor(WindowDescriptor descriptor)
{
    Window::SetDescriptor(descriptor);

    viewDescriptors[ApplicationViews::Settings] = ViewDescriptor(descriptor, false);
    viewDescriptors[ApplicationViews::Dictionary] = ViewDescriptor(descriptor, true);
    viewDescriptors[ApplicationViews::TranslateResult] = ViewDescriptor(descriptor, true);
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

    CreateChildWindows();

    Minimize();

    context->GetErrorHandler()->OnErrorShow.Subscribe(bind(&MainWindow::Minimize, this));
    context->GetMessageBus()->OnConfirmRequested.Subscribe(bind(&MainWindow::ShowConfirmDialog, this, placeholders::_1, placeholders::_2));
}

void MainWindow::CreateChildWindows()
{
    DestroyChildWindows();

    WindowDescriptor windowDescriptor = WindowDescriptor::CreateWindowDescriptor(
        Point(0, 0),
        GetAvailableClientSize(false),
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

    confirmDialogWindow = new ConfirmDialogWindow(context, this);
    confirmDialogWindow->SetDescriptor(WindowDescriptor::CreateFixedWindowDescriptor(Point(0, 0), GetAvailableClientSize()));
    AddChildWindow(confirmDialogWindow);
    confirmDialogWindow->MakeHidden();
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

void MainWindow::SetTranslateResultView(TranslateResult translateResult)
{
    AssertWindowInitialized();

    this->translateResult = translateResult;
    this->translationWindow->SetModel(translateResult);
    SetCurrentView(ApplicationViews::TranslateResult);
}

void MainWindow::SetDictionaryView(vector<DictionaryRecord> dictionaryRecords)
{
    AssertWindowInitialized();

    this->dictionaryRecords = dictionaryRecords;
    this->dictionaryWindow->SetModel(dictionaryRecords);
    SetCurrentView(ApplicationViews::Dictionary);
}

void MainWindow::SetSettingsView(Settings settings)
{
    AssertWindowInitialized();

    this->settings = settings;
    this->settingsWindow->SetModel(settings);
    SetCurrentView(ApplicationViews::Settings);
}

void MainWindow::SetCurrentView(ApplicationViews applicationView)
{
    AssertWindowInitialized();

    currentView = applicationView;

    descriptor = viewDescriptors[applicationView].GetWindowDescriptor();
    position = descriptor.GetPosition();
    windowSize = descriptor.GetWindowSize();
}

Size MainWindow::RenderContent(Renderer* renderer)
{
    if(currentView == ApplicationViews::None)
    {
        throw SelectedTextTranslateFatalException(L"View must set before rendering.");
    }

    for (size_t i = 0; i < activeChildWindows.size(); ++i)
    {
        activeChildWindows[i]->MakeHidden();
    }

    Window* windowToShow = GetWindowToShow();

    windowToShow->MakeVisible();
    windowToShow->Render();

    return windowToShow->GetContentSize();
}

void MainWindow::Scale(double scaleFactorAdjustment)
{
    ScaleViewDescriptor(ApplicationViews::Settings, scaleFactorAdjustment);
    ScaleViewDescriptor(ApplicationViews::Dictionary, scaleFactorAdjustment);
    ScaleViewDescriptor(ApplicationViews::TranslateResult, scaleFactorAdjustment);

    descriptor = viewDescriptors[currentView].GetWindowDescriptor();
    position = descriptor.GetPosition();
    windowSize = descriptor.GetWindowSize();

    context->GetScaleProvider()->AdjustScaleFactor(scaleFactorAdjustment);

    deviceContextBuffer->Resize(windowSize);

    CreateChildWindows();
    Render();
}

void MainWindow::ScaleViewDescriptor(ApplicationViews applicationView, double scaleFactorAdjustment)
{
    WindowDescriptor windowDescriptor = viewDescriptors[applicationView].GetWindowDescriptor();

    int scaledWidth = context->GetScaleProvider()->Rescale(windowDescriptor.GetWindowSize().Width, scaleFactorAdjustment);
    int scaledHeight = context->GetScaleProvider()->Rescale(windowDescriptor.GetWindowSize().Height, scaleFactorAdjustment);

    windowDescriptor.SetPosition(Point(
        windowDescriptor.GetPosition().X - scaledWidth + windowDescriptor.GetWindowSize().Width,
        windowDescriptor.GetPosition().Y - scaledHeight + windowDescriptor.GetWindowSize().Height));

    windowDescriptor.SetWindowSize(Size(scaledWidth, scaledHeight));

    viewDescriptors[applicationView].SetWindowDescriptor(windowDescriptor);
}

void MainWindow::Resize()
{
    if (windowState == WindowStates::Rendering)
    {
        return;
    }

    RECT windowRect;
    AssertCriticalWinApiResult(GetWindowRect(windowHandle, &windowRect));
    int newWidth = windowRect.right - windowRect.left;
    int newHeight = windowRect.bottom - windowRect.top;

    if (descriptor.GetWindowSize().Width == newWidth && descriptor.GetWindowSize().Height == newHeight)
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

    Window* currentWindow = GetWindowToShow();
    currentWindow->Resize();
    contentSize = currentWindow->GetContentSize();

    viewDescriptors[currentView].SetWindowDescriptor(descriptor);

    ApplyRenderedState(true);
}

Window* MainWindow::GetWindowToShow() const
{
    if (currentView == ApplicationViews::TranslateResult)
    {
        return translationWindow;
    }

    if (currentView == ApplicationViews::Dictionary)
    {
        return dictionaryWindow;
    }

    if (currentView == ApplicationViews::Settings)
    {
        return settingsWindow;
    }

    throw SelectedTextTranslateFatalException(StringUtilities::Format(L"Unsupported view: %d", currentView));
}

void MainWindow::ShowConfirmDialog(wstring title, function<void()> onConfirm)
{
    confirmDialogWindow->SetTitle(title);
    confirmDialogWindow->OnConfirm.UnsubscribeAll();
    confirmDialogWindow->OnConfirm.Subscribe(onConfirm);
    confirmDialogWindow->OnConfirm.Subscribe(bind(&MainWindow::ApplyWindowPosition, this, true));
    confirmDialogWindow->OnCancel.UnsubscribeAll();
    confirmDialogWindow->OnCancel.Subscribe(bind(&MainWindow::ApplyWindowPosition, this, true));
    confirmDialogWindow->Render();
    confirmDialogWindow->Show();
    context->GetScrollProvider()->HideScrollbars(this);
}

bool MainWindow::IsResizeLocked()
{
    return confirmDialogWindow->IsVisible() || !viewDescriptors[currentView].IsResizeable();
}

LRESULT MainWindow::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SIZE:
    {
        Resize();
        return TRUE;
    }

    case WM_SETCURSOR:
    {
        if (IsResizeLocked())
        {
            SetCursor(LoadCursor(nullptr, IDC_ARROW));
            return TRUE;
        }
    }

    case WM_SIZING:
    {
        if (IsResizeLocked())
        {
            RECT &newSize = *(LPRECT)lParam;
            RECT windowRect;
            GetWindowRect(windowHandle, &windowRect);
            newSize = windowRect;
            return 0;
        }
    }

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
        hotkeyProvider->ProcessHotkey(wParam);
        return Window::WindowProcedure(message, wParam, lParam);

    case WM_ACTIVATE:
    {
        if (wParam == WA_INACTIVE)
        {
            Minimize();
        }
        return TRUE;
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