#include "View\Controls\Dialogs\Confirm\ConfirmDialogWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "View\Controls\Dialogs\Confirm\ConfirmDialogContentWindow.h"
#include "View\Controls\Dialogs\Confirm\ConfirmDialogOverlayWindow.h"

ConfirmDialogWindow::ConfirmDialogWindow(ViewContext* context, View* parentView)
    : ChildView(context, parentView)
{
    this->className = L"STT_CONFIRM_DIALOG";

    this->dialogContentView = nullptr;
    this->title = wstring();
    this->OnConfirm = Subscribeable<>();
    this->OnCancel = Subscribeable<>();

    this->isLayered = false;
}

void ConfirmDialogWindow::Initialize()
{
    ChildView::Initialize();

    AssertCriticalWinApiResult(SetWindowPos(windowHandle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE));
}

void ConfirmDialogWindow::Show()
{
    ChildView::Show();
    AssertWinApiResult(SetFocus(windowHandle));
}

void ConfirmDialogWindow::SetTitle(wstring title)
{
    this->title = title;

    if(dialogContentView != nullptr)
    {
        dialogContentView->SetTitle(title);
        dialogContentView->Render();
    }
}

wstring ConfirmDialogWindow::GetTitle() const
{
    return title;
}

Size ConfirmDialogWindow::RenderContent(Renderer* renderer)
{
    DestroyChildViews();

    ConfirmDialogOverlayWindow* overlayWindow = new ConfirmDialogOverlayWindow(context, this);
    overlayWindow->SetDescriptor(WindowDescriptor::CreateFixedWindowDescriptor(Point(0, 0), GetSize()));
    overlayWindow->InitializeAndRender();

    int dialogContentWidth = scaleProvider->Scale(200);
    int paddingX = roundToInt((GetSize().GetWidth() - dialogContentWidth) / 2);
    int paddingY = scaleProvider->Scale(50);

    dialogContentView = new ConfirmDialogContentWindow(context, this);
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

ConfirmDialogWindow::~ConfirmDialogWindow()
{
}