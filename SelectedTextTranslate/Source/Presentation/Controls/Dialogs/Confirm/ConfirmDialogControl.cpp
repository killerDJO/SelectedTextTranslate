#include "Presentation\Controls\Dialogs\Confirm\ConfirmDialogControl.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Presentation\Controls\Dialogs\Confirm\ConfirmDialogContentControl.h"
#include "Presentation\Controls\Dialogs\Confirm\ConfirmDialogOverlayControl.h"

ConfirmDialogControl::ConfirmDialogControl(ViewContext* context, View* parentView)
    : ControlView(context, parentView)
{
    this->dialogContentView = nullptr;
    this->title = wstring();
    this->IsLayered = false;
}

void ConfirmDialogControl::Initialize()
{
    ControlView::Initialize();

    AssertCriticalWinApiResult(SetWindowPos(Window->GetHandle(), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE));
}

void ConfirmDialogControl::SpecifyWindow(NativeWindowHolder* window)
{
    window->SetClassName(L"STT_CONFIRM_DIALOG");
}

void ConfirmDialogControl::SetSize(Size size)
{
    AssertViewNotInitialized();
    ViewState->SetLayout(LayoutDescriptor::CreateFixedLayoutDescriptor(Point(0, 0), size));
}

void ConfirmDialogControl::Show()
{
    Window->Show();
    Window->Focus();
}

void ConfirmDialogControl::Hide()
{
    Window->Hide();
}

void ConfirmDialogControl::SetTitle(wstring title)
{
    this->title = title;

    if(dialogContentView != nullptr)
    {
        dialogContentView->SetTitle(title);
        dialogContentView->Render();
    }
}

wstring ConfirmDialogControl::GetTitle() const
{
    return title;
}

void ConfirmDialogControl::RenderContent(Renderer* renderer)
{
    DestroyChildViews();

    ConfirmDialogOverlayControl* overlayWindow = new ConfirmDialogOverlayControl(Context, this);
    overlayWindow->SetSize(ViewState->GetViewSize());
    overlayWindow->InitializeAndRender();

    int dialogContentWidth = ScaleProvider->Scale(200);
    int paddingX = roundToInt((ViewState->GetViewSize().GetWidth() - dialogContentWidth) / 2);
    int paddingY = ScaleProvider->Scale(50);

    dialogContentView = new ConfirmDialogContentControl(Context, this);
    dialogContentView->SetDimensions(Point(paddingX, paddingY), dialogContentWidth);
    dialogContentView->SetTitle(title);
    dialogContentView->OnConfirm.Subscribe(&OnConfirm);
    dialogContentView->OnConfirm.Subscribe(bind(&ConfirmDialogControl::Hide, this));
    dialogContentView->OnCancel.Subscribe(&OnCancel);
    dialogContentView->OnCancel.Subscribe(bind(&ConfirmDialogControl::Hide, this));
    dialogContentView->InitializeAndRender();
}

ConfirmDialogControl::~ConfirmDialogControl()
{
}
