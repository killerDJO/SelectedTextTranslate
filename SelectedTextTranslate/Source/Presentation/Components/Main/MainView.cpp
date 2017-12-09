#include "Presentation\Components\Main\MainView.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "Presentation\Controls\Dialogs\Confirm\ConfirmDialogControl.h"
#include "Presentation\Components\Main\Enums\ApplicationViews.h"
#include "Presentation\Components\Translation\TranslationComponent.h"
#include "Presentation\Framework\Providers\ScrollProvider.h"

MainView::MainView(ViewContext* context, ModelHolder<MainViewModel*>* modelHolder)
    : View(context)
{
    this->modelHolder = modelHolder;

    Name = L"MainWindow";
    confirmDialog = nullptr;
}

void MainView::Initialize()
{
    View::Initialize();

    CreateChildComponents();
    MakeHidden();
}

void MainView::SpecifyWindow(NativeWindowHolder* window)
{
    window
        ->SetClassName(L"STT_MAIN")
        ->AddExtendedStyles(WS_EX_TOOLWINDOW)
        ->AddStyles(WS_SIZEBOX | WS_POPUP)
        ->AddClassSpecifier(bind(&MainView::SpecifyWindowClass, this, _1))
        ->SetMessageHandler(WM_SIZE, bind(&MainView::Resize, this), TRUE)
        ->SetProxyMessageHandler(WM_SETCURSOR, bind(&MainView::ProcessSetCursor, this, _1, _2, _3))
        ->SetProxyMessageHandler(WM_SYSCOMMAND, bind(&MainView::ProcessSysCommand, this, _1, _2, _3))
        ->SetMessageHandler(WM_ACTIVATE, bind(&MainView::ProcessActivate, this, _1, _2))
        ->SetProxyMessageHandler(WM_HOTKEY, bind(&MainView::ProcessHotkey, this, _1, _2, _3))
        ->SetMessageHandler(WM_SHOWWINDOW, bind(&MainView::ProcessShowWindow, this, _1, _2));
}

void MainView::SetLayout(LayoutDescriptor layout) const
{
    ViewState->SetLayout(layout);
}

void MainView::Render(bool preserveScrolls)
{
    SetLayout(GetModel()->GetCurrentLayoutDescriptor());
    MakeVisible();
    View::Render(preserveScrolls);
}

void MainView::Hide()
{
    Window->Hide();
}

void MainView::CreateChildComponents()
{
    DestroyChildViews();

    InitializeComponent(new TranslationComponent(Context->GetServiceRegistry(), this), ApplicationViews::TranslateResult);
    InitializeComponent(new DictionaryComponent(Context->GetServiceRegistry(), this), ApplicationViews::Dictionary);
    InitializeComponent(new SettingsComponent(Context->GetServiceRegistry(), this), ApplicationViews::Settings);
    CreateConfirmDialog();
}

void MainView::InitializeComponent(IComponent* component, ApplicationViews view)
{
    component->SetLayout(LayoutDescriptor::CreateStretchViewLayout(Point(0, 0)));
    component->Initialize();
    component->MakeHidden();
    viewToComponentMap[view] = component;
}

void MainView::CreateConfirmDialog()
{
    confirmDialog = new ConfirmDialogControl(Context, this);
    confirmDialog->SetSize(GetAvailableClientSize());
    confirmDialog->Initialize();
    confirmDialog->MakeHidden();
}

void MainView::SpecifyWindowClass(WNDCLASSEX* windowClass)
{
    windowClass->hIcon = LoadIcon(windowClass->hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
    AssertCriticalWinApiResult(windowClass->hIcon);

    windowClass->hIconSm = LoadIcon(windowClass->hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
    AssertCriticalWinApiResult(windowClass->hIconSm);

    windowClass->hCursor = LoadCursor(nullptr, IDC_ARROW);
    AssertCriticalWinApiResult(windowClass->hCursor);
}

void MainView::RenderContent(Renderer* renderer)
{
    if (GetModel()->GetApplicationView() == ApplicationViews::None)
    {
        throw SelectedTextTranslateFatalException(L"View must set before rendering.");
    }

    for (auto viewToComponent : viewToComponentMap)
    {
        viewToComponent.second->MakeHidden();
    }

    IComponent* componentToShow = GetComponentToShow();
    componentToShow->MakeVisible();
    componentToShow->Render();
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
    if (ViewState->GetViewStatus() == ViewStatus::Rendering)
    {
        return;
    }

    Rect windowRect = Window->GetBoundingRect();
    if (ViewState->GetViewSize().Equals(windowRect.GetSize()))
    {
        return;
    }

    LayoutDescriptor layout = ViewState->GetLayout();
    layout.SetSize(windowRect.GetSize());
    layout.SetPosition(windowRect.GetPosition());
    GetModel()->GetViewDescriptor()->SetLayoutDescriptor(layout);

    Render(true);
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
    confirmDialog->MakeVisible();
    confirmDialog->Render();
    ScrollProvider->HideScrollbars(this);
    confirmDialog->Show();
}

bool MainView::IsResizeLocked() const
{
    return confirmDialog->IsVisible() || !GetModel()->GetViewDescriptor()->IsResizeable();
}

LRESULT MainView::ProcessSysCommand(WPARAM wParam, LPARAM lParam, function<LRESULT()> baseProcedure)
{
    switch (wParam & 0xfff0)
    {
    case SC_MINIMIZE:
    case SC_CLOSE:
        Hide();
        return 0;
    default:
        break;
    }

    return baseProcedure();
}

LRESULT MainView::ProcessActivate(WPARAM wParam, LPARAM lParam)
{
    if (wParam == WA_INACTIVE)
    {
        Hide();
    }

    return TRUE;
}

LRESULT MainView::ProcessSetCursor(WPARAM wParam, LPARAM lParam, function<LRESULT()> baseProcedure)
{
    if (IsResizeLocked())
    {
        SetCursor(LoadCursor(nullptr, IDC_ARROW));
        return TRUE;
    }

    return baseProcedure();
}

LRESULT MainView::ProcessHotkey(WPARAM wParam, LPARAM lParam, function<LRESULT()> baseProcedure)
{
    OnHotkey(wParam);
    return baseProcedure();
}

LRESULT MainView::ProcessShowWindow(WPARAM wParam, LPARAM lParam)
{
    OnVisibilityChanged(wParam == TRUE);
    return 0;
}
