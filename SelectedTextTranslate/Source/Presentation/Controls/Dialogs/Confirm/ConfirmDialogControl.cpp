#include "Presentation\Controls\Dialogs\Confirm\ConfirmDialogControl.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Presentation\Controls\Dialogs\Confirm\ConfirmDialogContentControl.h"
#include "Presentation\Controls\Dialogs\Confirm\ConfirmDialogOverlayControl.h"

ConfirmDialogControl::ConfirmDialogControl(CommonContext* context, View* parentView)
    : ControlView(context, parentView)
{
    this->className = L"STT_CONFIRM_DIALOG";

    this->dialogContentView = nullptr;
    this->title = wstring();
    this->OnConfirm = Subscribeable<>();
    this->OnCancel = Subscribeable<>();

    this->isLayered = false;
}

void ConfirmDialogControl::Initialize()
{
    ControlView::Initialize();

    AssertCriticalWinApiResult(SetWindowPos(windowHandle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE));
}

void ConfirmDialogControl::SetSize(Size size)
{
    AssertViewNotInitialized();
    layoutDescriptor = LayoutDescriptor::CreateFixedLayoutDescriptor(Point(0, 0), size);
}

void ConfirmDialogControl::Show()
{
    ControlView::Show();
    AssertWinApiResult(SetFocus(windowHandle));
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

    ConfirmDialogOverlayControl* overlayWindow = new ConfirmDialogOverlayControl(context, this);
    overlayWindow->SetSize(GetSize());
    overlayWindow->InitializeAndRender();

    int dialogContentWidth = scaleProvider->Scale(200);
    int paddingX = roundToInt((GetSize().GetWidth() - dialogContentWidth) / 2);
    int paddingY = scaleProvider->Scale(50);

    dialogContentView = new ConfirmDialogContentControl(context, this);
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