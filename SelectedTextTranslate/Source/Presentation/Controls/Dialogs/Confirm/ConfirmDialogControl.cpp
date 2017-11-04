#include "Presentation\Controls\Dialogs\Confirm\ConfirmDialogControl.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Presentation\Controls\Dialogs\Confirm\ConfirmDialogContentControl.h"
#include "Presentation\Controls\Dialogs\Confirm\ConfirmDialogOverlayControl.h"

ConfirmDialogControl::ConfirmDialogControl(ViewContext* context, View* parentView)
    : ControlView(context, parentView)
{
    this->ClassName = L"STT_CONFIRM_DIALOG";

    this->dialogContentView = nullptr;
    this->title = wstring();
    this->OnConfirm = Subscribable<>();
    this->OnCancel = Subscribable<>();

    this->IsLayered = false;
}

void ConfirmDialogControl::Initialize()
{
    ControlView::Initialize();

    AssertCriticalWinApiResult(SetWindowPos(Handle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE));
}

void ConfirmDialogControl::SetSize(Size size)
{
    AssertViewNotInitialized();
    State->SetLayout(LayoutDescriptor::CreateFixedLayoutDescriptor(Point(0, 0), size));
}

void ConfirmDialogControl::Show()
{
    ControlView::Show();
    AssertWinApiResult(SetFocus(Handle));
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

Size ConfirmDialogControl::RenderContent(Renderer* renderer)
{
    DestroyChildViews();

    ConfirmDialogOverlayControl* overlayWindow = new ConfirmDialogOverlayControl(Context, this);
    overlayWindow->SetSize(State->GetWindowSize());
    overlayWindow->InitializeAndRender();

    int dialogContentWidth = ScaleProvider->Scale(200);
    int paddingX = roundToInt((State->GetWindowSize().GetWidth() - dialogContentWidth) / 2);
    int paddingY = ScaleProvider->Scale(50);

    dialogContentView = new ConfirmDialogContentControl(Context, this);
    dialogContentView->SetDimensions(Point(paddingX, paddingY), dialogContentWidth);
    dialogContentView->SetTitle(title);
    dialogContentView->OnConfirm.Subscribe(&OnConfirm);
    dialogContentView->OnConfirm.Subscribe(bind(&View::Hide, this));
    dialogContentView->OnCancel.Subscribe(&OnCancel);
    dialogContentView->OnCancel.Subscribe(bind(&View::Hide, this));
    dialogContentView->InitializeAndRender();

    renderer->UpdateRenderedContentSize(overlayWindow);
    renderer->UpdateRenderedContentSize(dialogContentView);

    return renderer->GetSize();
}

ConfirmDialogControl::~ConfirmDialogControl()
{
}