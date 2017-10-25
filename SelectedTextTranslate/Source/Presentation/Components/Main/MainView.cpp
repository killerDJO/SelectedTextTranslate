#include "Presentation\Components\Main\MainView.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "Utilities\StringUtilities.h"
#include "Presentation\Controls\Dialogs\Confirm\ConfirmDialogControl.h"
#include "Presentation\Components\Main\Enums\ApplicationViews.h"
#include "Presentation\Components\Translation\TranslationComponent.h"
#include "Presentation\Framework\Providers\ScrollProvider.h"
#include "Presentation\MessageBus.h"

MainView::MainView(CommonContext* context)
    : View(context)
{
    this->ClassName = L"STT_MAIN";
    this->ViewName = L"MainWindow";
    this->applicationView = ApplicationViews::None;

    this->dictionaryComponent = nullptr;
    this->translationComponent = nullptr;
    this->settingsComponent = nullptr;
    this->confirmDialogWindow = nullptr;

    this->applicationViewDescriptors = map<ApplicationViews, ViewDescriptor>();
}

void MainView::SetLayout(::LayoutDescriptor layoutDescriptor)
{
    this->LayoutDescriptor = layoutDescriptor;

    applicationViewDescriptors[ApplicationViews::Settings] = ViewDescriptor(layoutDescriptor, false);
    applicationViewDescriptors[ApplicationViews::Dictionary] = ViewDescriptor(layoutDescriptor, true);
    applicationViewDescriptors[ApplicationViews::TranslateResult] = ViewDescriptor(layoutDescriptor, true);
    minSize = layoutDescriptor.GetSize();
}

void MainView::Initialize()
{
    View::Initialize();

    Handle = CreateWindowEx(
        WS_EX_TOOLWINDOW,
        ClassName,
        nullptr,
        WS_SIZEBOX | WS_POPUP | WS_CLIPCHILDREN | GetScrollStyle(),
        LayoutDescriptor.GetPosition().GetX(),
        LayoutDescriptor.GetPosition().GetY(),
        LayoutDescriptor.GetSize().GetWidth(),
        LayoutDescriptor.GetSize().GetHeight(),
        nullptr,
        nullptr,
        Context->GetInstance(),
        nullptr);
    AssertCriticalWinApiResult(Handle);
    SetWindowLongPtr(Handle, GWLP_USERDATA, (LONG_PTR)this);

    CreateChildComponents();

    Minimize();

    Context->GetErrorHandler()->OnErrorShow.Subscribe(bind(&MainView::Minimize, this));
    Context->Get<MessageBus>()->OnConfirmRequested.Subscribe(bind(&MainView::ShowConfirmDialog, this, placeholders::_1, placeholders::_2));
}

void MainView::Render(bool preserveScrolls)
{
    View::Render(preserveScrolls);
    Maximize();
}

void MainView::CreateChildComponents()
{
    DestroyChildViews();

    translationComponent = new TranslationComponent(Context, this);
    SetViewWindowDescriptor(translationComponent, ApplicationViews::TranslateResult);
    translationComponent->MakeHidden();
    translationComponent->Initialize();

    dictionaryComponent = new DictionaryComponent(Context, this);
    SetViewWindowDescriptor(dictionaryComponent, ApplicationViews::Dictionary);
    dictionaryComponent->OnShowTranslation.Subscribe([this](wstring input)
    {
        this->translationComponent->Translate(input, false);
        SetApplicationView(ApplicationViews::TranslateResult);
        Render();
    });
    dictionaryComponent->MakeHidden();
    dictionaryComponent->Initialize();

    settingsComponent = new SettingsComponent(Context, this);
    SetViewWindowDescriptor(settingsComponent, ApplicationViews::Settings);
    settingsComponent->MakeHidden();
    settingsComponent->Initialize();

    confirmDialogWindow = new ConfirmDialogControl(Context, this);
    confirmDialogWindow->SetSize(GetClientSize());
    confirmDialogWindow->MakeHidden();
    confirmDialogWindow->Initialize();
}

void MainView::SetViewWindowDescriptor(IComponent* component, ApplicationViews view)
{
    ::LayoutDescriptor windowDescriptor = LayoutDescriptor::CreateLayoutDescriptor(
        Point(0, 0),
        applicationViewDescriptors[view].GetWindowDescriptor().GetSize(),
        OverflowModes::Stretch,
        OverflowModes::Stretch);
    component->SetLayout(windowDescriptor);
}

void MainView::SpecifyWindowClass(WNDCLASSEX* windowClass)
{
    windowClass->hIcon = LoadIcon(Context->GetInstance(), MAKEINTRESOURCE(IDI_APP_ICON));
    AssertCriticalWinApiResult(windowClass->hIcon);

    windowClass->hIconSm = LoadIcon(Context->GetInstance(), MAKEINTRESOURCE(IDI_APP_ICON));
    AssertCriticalWinApiResult(windowClass->hIconSm);

    windowClass->hCursor = LoadCursor(nullptr, IDC_ARROW);
    AssertCriticalWinApiResult(windowClass->hCursor);

    windowClass->hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    AssertCriticalWinApiResult(windowClass->hbrBackground);
}

void MainView::Minimize()
{
    Hide();
}

void MainView::Maximize()
{
    Show();
    SwitchToThisWindow(Handle, TRUE);
}

void MainView::SetApplicationView(ApplicationViews applicationView)
{
    AssertViewInitialized();

    this->applicationView = applicationView;

    LayoutDescriptor = applicationViewDescriptors[applicationView].GetWindowDescriptor();
    ViewStateDescriptor.SetPosition(LayoutDescriptor.GetPosition());
    ViewStateDescriptor.SetSize(LayoutDescriptor.GetSize());
}

void MainView::Translate(wstring input)
{
    SetApplicationView(ApplicationViews::TranslateResult);
    this->translationComponent->Translate(input, true);
}

Size MainView::RenderContent(Renderer* renderer)
{
    if (applicationView == ApplicationViews::None)
    {
        throw SelectedTextTranslateFatalException(L"View must set before rendering.");
    }

    for (size_t i = 0; i < ActiveChildViews.size(); ++i)
    {
        ActiveChildViews[i]->MakeHidden();
    }

    IComponent* componentToShow = GetComponentToShow();

    componentToShow->MakeVisible();
    componentToShow->Render();

    return componentToShow->GetBoundingRect().GetSize();
}

void MainView::Scale(double scaleFactorAdjustment)
{
    if (!ScaleProvider->IsScalingAllowed(scaleFactorAdjustment))
    {
        return;
    }

    // TODO: iterate thru map
    ScaleViewDescriptor(ApplicationViews::Settings, scaleFactorAdjustment);
    ScaleViewDescriptor(ApplicationViews::Dictionary, scaleFactorAdjustment);
    ScaleViewDescriptor(ApplicationViews::TranslateResult, scaleFactorAdjustment);
    minSize = Size(
        ScaleProvider->Rescale(minSize.GetWidth(), scaleFactorAdjustment),
        ScaleProvider->Rescale(minSize.GetHeight(), scaleFactorAdjustment));

    LayoutDescriptor = applicationViewDescriptors[applicationView].GetWindowDescriptor();
    ViewStateDescriptor.SetPosition(LayoutDescriptor.GetPosition());
    ViewStateDescriptor.SetSize(LayoutDescriptor.GetSize());

    ScaleProvider->AdjustScaleFactor(scaleFactorAdjustment);

    DeviceContextBuffer->Resize(ViewStateDescriptor.GetSize());

    CreateChildComponents();
    Render();
}

void MainView::ScaleViewDescriptor(ApplicationViews applicationView, double scaleFactorAdjustment)
{
    ::LayoutDescriptor windowDescriptor = applicationViewDescriptors[applicationView].GetWindowDescriptor();

    int scaledWidth = ScaleProvider->Rescale(windowDescriptor.GetSize().GetWidth(), scaleFactorAdjustment);
    int scaledHeight = ScaleProvider->Rescale(windowDescriptor.GetSize().GetHeight(), scaleFactorAdjustment);

    windowDescriptor.SetPosition(Point(
        windowDescriptor.GetPosition().GetX() - scaledWidth + windowDescriptor.GetSize().GetWidth(),
        windowDescriptor.GetPosition().GetY() - scaledHeight + windowDescriptor.GetSize().GetHeight()));

    windowDescriptor.SetSize(Size(scaledWidth, scaledHeight));

    applicationViewDescriptors[applicationView].SetWindowDescriptor(windowDescriptor);
}

void MainView::Resize()
{
    if (ViewStateDescriptor.GetViewState() == ViewStates::Rendering)
    {
        return;
    }

    RECT windowRect;
    AssertCriticalWinApiResult(GetWindowRect(Handle, &windowRect));
    int newWidth = windowRect.right - windowRect.left;
    int newHeight = windowRect.bottom - windowRect.top;

    if (LayoutDescriptor.GetSize().GetWidth() == newWidth && LayoutDescriptor.GetSize().GetHeight() == newHeight)
    {
        return;
    }

    LayoutDescriptor.SetSize(Size(newWidth, newHeight));
    ViewStateDescriptor.SetSize(LayoutDescriptor.GetSize());

    LayoutDescriptor.SetPosition(Point(windowRect.left, windowRect.top));
    ViewStateDescriptor.SetPosition(LayoutDescriptor.GetPosition());

    DeviceContextBuffer->Resize(ViewStateDescriptor.GetSize());

    // Clear background
    Renderer* renderer = RenderingContext->GetRenderer();
    renderer->Render(DeviceContextBuffer);
    RenderingContext->ReleaseRenderer(renderer);

    IComponent* currentComponent = GetComponentToShow();
    currentComponent->Resize();
    ViewStateDescriptor.SetContentSize(currentComponent->GetBoundingRect().GetSize());

    applicationViewDescriptors[applicationView].SetWindowDescriptor(LayoutDescriptor);

    ApplyViewState(true);
}

IComponent* MainView::GetComponentToShow() const
{
    if (applicationView == ApplicationViews::TranslateResult)
    {
        return translationComponent;
    }

    if (applicationView == ApplicationViews::Dictionary)
    {
        return dictionaryComponent;
    }

    if (applicationView == ApplicationViews::Settings)
    {
        return settingsComponent;
    }

    throw SelectedTextTranslateFatalException(StringUtilities::Format(L"Unsupported view: %d", applicationView));
}

void MainView::ShowConfirmDialog(wstring title, function<void()> onConfirm)
{
    confirmDialogWindow->SetTitle(title);
    confirmDialogWindow->OnConfirm.UnsubscribeAll();
    confirmDialogWindow->OnConfirm.Subscribe(onConfirm);
    confirmDialogWindow->OnConfirm.Subscribe(bind(&MainView::ApplyViewPosition, this, true));
    confirmDialogWindow->OnCancel.UnsubscribeAll();
    confirmDialogWindow->OnCancel.Subscribe(bind(&MainView::ApplyViewPosition, this, true));
    confirmDialogWindow->Render();
    confirmDialogWindow->Show();
    ScrollProvider->HideScrollbars(this);
}

bool MainView::IsResizeLocked()
{
    return confirmDialogWindow->IsVisible() || !applicationViewDescriptors[applicationView].IsResizeable();
}

LRESULT MainView::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
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

        return View::WindowProcedure(message, wParam, lParam);
    }

    case WM_SIZING:
    {
        RECT &newSize = *(LPRECT)lParam;
        int newWidth = newSize.right - newSize.left;
        int newHeight = newSize.bottom - newSize.top;

        if (IsResizeLocked() || newWidth < minSize.GetWidth() || newHeight < minSize.GetHeight())
        {
            RECT windowRect;
            GetWindowRect(Handle, &windowRect);
            newSize = windowRect;
            return 0;
        }

        return TRUE;
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

        return View::WindowProcedure(message, wParam, lParam);
    }

    case WM_HOTKEY:
    {
        OnHotkey.Notify(wParam);
        return View::WindowProcedure(message, wParam, lParam);
    }

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
        OnVisibilityChanged.Notify(wParam == TRUE);
        break;
    }

    default:
        return View::WindowProcedure(message, wParam, lParam);
    }

    return 0;
}