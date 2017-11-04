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

    ClassName = L"STT_MAIN";
    Name = L"MainWindow";
    confirmDialog = nullptr;
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

void MainView::SetLayout(LayoutDescriptor layout) const
{
    State->SetLayout(layout);
}

void MainView::Render(bool preserveScrolls)
{
    SetLayout(GetModel()->GetCurrentLayoutDescriptor());
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
    if (State->GetViewState() == ViewStates::Rendering)
    {
        return;
    }

    RECT windowRect;
    AssertCriticalWinApiResult(GetWindowRect(Handle, &windowRect));
    Size newSize = Size(
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top);

    if (State->GetViewSize().Equals(newSize))
    {
        return;
    }

    LayoutDescriptor layout = State->GetLayout();
    layout.SetSize(newSize);
    layout.SetPosition(Point(windowRect.left, windowRect.top));
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
}

bool MainView::IsResizeLocked() const
{
    return confirmDialog->IsVisible() || !GetModel()->GetViewDescriptor()->IsResizeable();
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
        Resize();

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
