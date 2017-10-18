#include "View\Controls\Dialogs\Confirm\ConfirmDialogOverlayWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

ConfirmDialogOverlayWindow::ConfirmDialogOverlayWindow(ViewContext* context, View* parentWindow)
    : ChildView(context, parentWindow)
{
    this->className = L"STT_CONFIRM_DIALOG_OVERLAY";
    this->isLayered = true;
    this->backgroundBrush = renderingProvider->CreateCustomBrush(Colors::White);
}

void ConfirmDialogOverlayWindow::Initialize()
{
    ChildView::Initialize();

    AssertCriticalWinApiResult(SetLayeredWindowAttributes(windowHandle, 0, 175, LWA_ALPHA));
}

Size ConfirmDialogOverlayWindow::RenderContent(Renderer* renderer)
{
    renderer->DrawRect(Rect(Point(0, 0), nativeStateDescriptor.GetSize()), backgroundBrush);
    return nativeStateDescriptor.GetSize();
}

ConfirmDialogOverlayWindow::~ConfirmDialogOverlayWindow()
{
    delete backgroundBrush;
}