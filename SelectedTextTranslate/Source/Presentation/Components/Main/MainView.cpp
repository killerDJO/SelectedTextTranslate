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
    this->ClassName = L"STT_MAIN";
    this->Name = L"MainWindow";

    this->modelHolder = modelHolder;

    this->confirmDialog = nullptr;
}

void MainView::Initialize()
{
    View::Initialize();

    CreateChildComponents();
    MakeHidden();
}

DWORD MainView::GetExtendedWindowStyles() const
{
    return WS_EX_TOOLWINDOW;
}

DWORD MainView::GetWindowStyle() const
{
    return View::GetWindowStyle() | WS_SIZEBOX | WS_POPUP;
}

void MainView::SetLayout(LayoutDescriptor layout)
{
    Layout = layout;
}

void MainView::Render(bool preserveScrolls)
{
    Layout = GetModel()->GetCurrentLayoutDescriptor();
    State.SetPosition(Layout.GetPosition());
    State.SetSize(Layout.GetSize());
    MakeVisible();
    View::Render(preserveScrolls);
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
    LayoutDescriptor initialLayoutDescriptor = LayoutDescriptor::CreateLayoutDescriptor(
        Point(0, 0),
        Size(GetAvailableClientSize().GetWidth(), 0),
        OverflowModes::Stretch,
        OverflowModes::Stretch);
    component->SetLayout(initialLayoutDescriptor);
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
    View::SpecifyWindowClass(windowClass);

    windowClass->hIcon = LoadIcon(Instance, MAKEINTRESOURCE(IDI_APP_ICON));
    AssertCriticalWinApiResult(windowClass->hIcon);

    windowClass->hIconSm = LoadIcon(Instance, MAKEINTRESOURCE(IDI_APP_ICON));
    AssertCriticalWinApiResult(windowClass->hIconSm);

    windowClass->hCursor = LoadCursor(nullptr, IDC_ARROW);
    AssertCriticalWinApiResult(windowClass->hCursor);

    windowClass->hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    AssertCriticalWinApiResult(windowClass->hbrBackground);
}

void MainView::Show()
{
    View::Show();
    SwitchToThisWindow(Handle, TRUE);
}

Size MainView::RenderContent(Renderer* renderer)
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
            Hide();
            return 0;
        default:
            break;
        }

        return View::WindowProcedure(message, wParam, lParam);
    }

    case WM_ACTIVATE:
    {
        if (wParam == WA_INACTIVE)
        {
            Hide();
        }
        return TRUE;
    }

    case WM_HOTKEY:
    {
        OnHotkey(wParam);
        return View::WindowProcedure(message, wParam, lParam);
    }

    case WM_SHOWWINDOW:
    {
        OnVisibilityChanged(wParam == TRUE);
        break;
    }

    default:
        return View::WindowProcedure(message, wParam, lParam);
    }

    return 0;
}
