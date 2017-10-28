#include "Presentation\Components\Main\MainView.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "Presentation\Controls\Dialogs\Confirm\ConfirmDialogControl.h"
#include "Presentation\Components\Main\Enums\ApplicationViews.h"
#include "Presentation\Components\Translation\TranslationComponent.h"
#include "Presentation\Framework\Providers\ScrollProvider.h"
#include "Presentation\MessageBus.h"

MainView::MainView(CommonContext* context, ModelHolder<MainViewModel*>* modelHolder)
    : View(context)
{
    this->ClassName = L"STT_MAIN";
    this->Name = L"MainWindow";

    this->modelHolder = modelHolder;

    this->dictionaryComponent = nullptr;
    this->translationComponent = nullptr;
    this->settingsComponent = nullptr;
    this->confirmDialog = nullptr;
}

void MainView::Initialize()
{
    View::Initialize();

    Handle = CreateWindowEx(
        WS_EX_TOOLWINDOW,
        ClassName,
        nullptr,
        WS_SIZEBOX | WS_POPUP | WS_CLIPCHILDREN | GetScrollStyle(),
        Layout.GetPosition().GetX(),
        Layout.GetPosition().GetY(),
        Layout.GetSize().GetWidth(),
        Layout.GetSize().GetHeight(),
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

void MainView::SetLayout(::LayoutDescriptor layout)
{
    Layout = layout;
}

void MainView::Render(bool preserveScrolls)
{
    Layout = GetModel()->GetCurrentLayoutDescriptor();
    State.SetPosition(Layout.GetPosition());
    State.SetSize(Layout.GetSize());
    View::Render(preserveScrolls);
    Maximize();
}

void MainView::CreateChildComponents()
{
    DestroyChildViews();

    translationComponent = new TranslationComponent(Context, this);
    InitializeComponent(translationComponent, ApplicationViews::TranslateResult);

    dictionaryComponent = new DictionaryComponent(Context, this);
    dictionaryComponent->OnShowTranslation.Subscribe([this](wstring input)
    {
        translationComponent->Translate(input, false);
        GetModel()->SetApplicationView(ApplicationViews::TranslateResult);
        Render();
    });
    InitializeComponent(dictionaryComponent, ApplicationViews::Dictionary);

    settingsComponent = new SettingsComponent(Context, this);
    InitializeComponent(settingsComponent, ApplicationViews::Settings);

    confirmDialog = new ConfirmDialogControl(Context, this);
    confirmDialog->SetSize(GetClientSize());
    confirmDialog->MakeHidden();
    confirmDialog->Initialize();
}

void MainView::InitializeComponent(IComponent* component, ApplicationViews view)
{
    ::LayoutDescriptor initialLayoutDescriptor = LayoutDescriptor::CreateLayoutDescriptor(
        Point(0, 0),
        GetModel()->GetLayoutDescriptor(view).GetSize(),
        OverflowModes::Stretch,
        OverflowModes::Stretch);
    component->SetLayout(initialLayoutDescriptor);
    component->MakeHidden();
    component->Initialize();
    viewToComponentMap[view] = component;
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

void MainView::Translate(wstring input) const
{
    GetModel()->SetApplicationView(ApplicationViews::TranslateResult);
    translationComponent->Translate(input, true);
}

Size MainView::RenderContent(Renderer* renderer)
{
    if (GetModel()->GetApplicationView() == ApplicationViews::None)
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

    GetModel()->Scale(scaleFactorAdjustment, ScaleProvider);
    ScaleProvider->AdjustScaleFactor(scaleFactorAdjustment);
    CreateChildComponents();
    Render();
}

void MainView::Resize()
{
    if (State.GetViewState() == ViewStates::Rendering)
    {
        return;
    }

    RECT windowRect;
    AssertCriticalWinApiResult(GetWindowRect(Handle, &windowRect));
    int newWidth = windowRect.right - windowRect.left;
    int newHeight = windowRect.bottom - windowRect.top;

    if (Layout.GetSize().GetWidth() == newWidth && Layout.GetSize().GetHeight() == newHeight)
    {
        return;
    }

    Layout.SetSize(Size(newWidth, newHeight));
    State.SetSize(Layout.GetSize());

    Layout.SetPosition(Point(windowRect.left, windowRect.top));
    State.SetPosition(Layout.GetPosition());

    DeviceContextBuffer->Resize(State.GetSize());

    // Clear background
    Renderer* renderer = RenderingContext->GetRenderer();
    renderer->Render(DeviceContextBuffer);
    RenderingContext->ReleaseRenderer(renderer);

    IComponent* currentComponent = GetComponentToShow();
    currentComponent->Resize();
    State.SetContentSize(currentComponent->GetBoundingRect().GetSize());

    GetModel()->GetViewDescriptor().SetLayoutDescriptor(Layout);

    ApplyViewState(true);
}

IComponent* MainView::GetComponentToShow()
{
    return viewToComponentMap[GetModel()->GetApplicationView()];
}

MainViewModel* MainView::GetModel() const
{
    return modelHolder->GetModel();
}

void MainView::ShowConfirmDialog(wstring title, function<void()> onConfirm)
{
    confirmDialog->SetTitle(title);
    confirmDialog->OnConfirm.UnsubscribeAll();
    confirmDialog->OnConfirm.Subscribe(onConfirm);
    confirmDialog->OnConfirm.Subscribe(bind(&MainView::ApplyViewPosition, this, true));
    confirmDialog->OnCancel.UnsubscribeAll();
    confirmDialog->OnCancel.Subscribe(bind(&MainView::ApplyViewPosition, this, true));
    confirmDialog->Render();
    confirmDialog->Show();
    ScrollProvider->HideScrollbars(this);
}

bool MainView::IsResizeLocked() const
{
    return confirmDialog->IsVisible() || !GetModel()->GetViewDescriptor().IsResizeable();
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

        Size minimumSize = GetModel()->GetMinimumSize();
        if (IsResizeLocked() || newWidth < minimumSize.GetWidth() || newHeight < minimumSize.GetHeight())
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