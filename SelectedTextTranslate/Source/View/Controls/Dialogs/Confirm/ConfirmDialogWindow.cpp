#include "View\Controls\Dialogs\Confirm\ConfirmDialogWindow.h"
#include "View\Controls\Dialogs\Confirm\ConfirmDialogContentWindow.h"
#include "View\Controls\Dialogs\Confirm\ConfirmDialogOverlayWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

ConfirmDialogWindow::ConfirmDialogWindow(WindowContext* context, Window* parentWindow)
    : ChildWindow(context, parentWindow)
{
    this->className = L"STT_CONFIRM_DIALOG";
}

Size ConfirmDialogWindow::RenderContent(Renderer* renderer)
{
    DestroyChildWindows();

    ConfirmDialogOverlayWindow* overlayWindow = new ConfirmDialogOverlayWindow(context, this);
    overlayWindow->SetDescriptor(WindowDescriptor::CreateFixedWindowDescriptor(Point(0, 0), windowSize));
    AddChildWindow(overlayWindow);
    overlayWindow->Render();

    ConfirmDialogContentWindow* dialogContentWindow = new ConfirmDialogContentWindow(context, this);

    Size dialogContentSize = Size(220, 100);
    int paddingX = roundToInt((windowSize.Width - dialogContentSize.Width) / 2);
    int paddingY = 50;
    dialogContentWindow->SetDescriptor(WindowDescriptor::CreateFixedWindowDescriptor(Point(paddingX, paddingY), dialogContentSize));
    dialogContentWindow->SetText(L"Reset Settings?");
    AddChildWindow(dialogContentWindow);
    dialogContentWindow->Render();

    renderer->UpdateRenderedContentSize(dialogContentWindow);

    return renderer->GetScaledSize();
}

void ConfirmDialogWindow::Initialize()
{
    ChildWindow::Initialize();

    AssertCriticalWinApiResult(SetWindowPos(windowHandle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE));

    Render();
}

ConfirmDialogWindow::~ConfirmDialogWindow()
{
}