#include "Presentation\Controls\Dialogs\Confirm\ConfirmDialogOverlayWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

ConfirmDialogOverlayWindow::ConfirmDialogOverlayWindow(CommonContext* context, View* parentWindow)
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

void ConfirmDialogOverlayWindow::SetSize(Size size)
{
    AssertViewNotInitialized();
    layoutDescriptor = LayoutDescriptor::CreateFixedLayoutDescriptor(Point(0, 0), size);
}

Size ConfirmDialogOverlayWindow::RenderContent(Renderer* renderer)
{
    renderer->DrawRect(Rect(Point(0, 0), viewStateDescriptor.GetSize()), backgroundBrush);
    return viewStateDescriptor.GetSize();
}

ConfirmDialogOverlayWindow::~ConfirmDialogOverlayWindow()
{
    delete backgroundBrush;
}
