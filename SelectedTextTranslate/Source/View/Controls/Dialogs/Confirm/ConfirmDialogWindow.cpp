#include "View\Controls\Dialogs\Confirm\ConfirmDialogWindow.h"
#include "View\Controls\Dialogs\Confirm\ConfirmDialogContentWindow.h"
#include "View\Controls\Dialogs\Confirm\ConfirmDialogOverlayWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

ConfirmDialogWindow::ConfirmDialogWindow(WindowContext* context, Window* parentWindow)
    : ChildWindow(context, parentWindow)
{
    this->className = L"STT_CONFIRM_DIALOG";

    this->dialogContentWindow = nullptr;
    this->title = wstring();
    this->OnConfirm = Subscribeable<>();
    this->OnCancel = Subscribeable<>();

    this->isLayered = false;
}

void ConfirmDialogWindow::Initialize()
{
    ChildWindow::Initialize();

    AssertCriticalWinApiResult(SetWindowPos(windowHandle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE));
}

void ConfirmDialogWindow::Show()
{
    ChildWindow::Show();
    AssertWinApiResult(SetFocus(windowHandle));
}

void ConfirmDialogWindow::SetTitle(wstring title)
{
    this->title = title;

    if(dialogContentWindow != nullptr)
    {
        dialogContentWindow->SetTitle(title);
        dialogContentWindow->Render();
    }
}

wstring ConfirmDialogWindow::GetTitle() const
{
    return title;
}

Size ConfirmDialogWindow::RenderContent(Renderer* renderer)
{
    DestroyChildWindows();

    ConfirmDialogOverlayWindow* overlayWindow = new ConfirmDialogOverlayWindow(context, this);
    overlayWindow->SetDescriptor(WindowDescriptor::CreateFixedWindowDescriptor(Point(0, 0), GetScaledSize()));
    AddChildWindow(overlayWindow);
    overlayWindow->Render();

    dialogContentWindow = new ConfirmDialogContentWindow(context, this);

    int dialogWidth = 200;
    double paddingX = (GetDownscaledSize().GetWidth() - dialogWidth) / 2;
    int paddingY = 50;
    dialogContentWindow->SetDimensions(PointReal(paddingX, paddingY), dialogWidth);
    dialogContentWindow->SetTitle(title);
    dialogContentWindow->OnConfirm.Subscribe(&OnConfirm);
    dialogContentWindow->OnConfirm.Subscribe(bind(&Window::Hide, this));
    dialogContentWindow->OnCancel.Subscribe(&OnCancel);
    dialogContentWindow->OnCancel.Subscribe(bind(&Window::Hide, this));
    AddChildWindow(dialogContentWindow);
    dialogContentWindow->Render();

    renderer->UpdateRenderedContentSize(dialogContentWindow);

    return renderer->GetScaledSize();
}

ConfirmDialogWindow::~ConfirmDialogWindow()
{
}