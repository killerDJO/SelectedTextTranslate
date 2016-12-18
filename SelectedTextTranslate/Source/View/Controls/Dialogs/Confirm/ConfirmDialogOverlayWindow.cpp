#include "View\Controls\Dialogs\Confirm\ConfirmDialogOverlayWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

ConfirmDialogOverlayWindow::ConfirmDialogOverlayWindow(WindowContext* context, Window* parentWindow)
    : ChildWindow(context, parentWindow)
{
    this->className = L"STT_CONFIRM_DIALOG_OVERLAY";
    this->isLayered = true;
}

void ConfirmDialogOverlayWindow::Initialize()
{
    ChildWindow::Initialize();

    AssertCriticalWinApiResult(SetLayeredWindowAttributes(windowHandle, 0, 175, LWA_ALPHA));
}

Size ConfirmDialogOverlayWindow::RenderContent(Renderer* renderer)
{
    renderer->DrawRect(RectReal(PointReal(0, 0), context->GetScaleProvider()->Downscale(GetSize())), (HBRUSH)GetStockObject(WHITE_BRUSH));
    return windowSize;
}

ConfirmDialogOverlayWindow::~ConfirmDialogOverlayWindow()
{
}