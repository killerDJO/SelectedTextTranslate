#include "Presentation\Controls\Dialogs\Confirm\ConfirmDialogOverlayControl.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

ConfirmDialogOverlayControl::ConfirmDialogOverlayControl(CommonContext* context, View* parentWindow)
    : ControlView(context, parentWindow)
{
    this->className = L"STT_CONFIRM_DIALOG_OVERLAY";
    this->isLayered = true;
    this->backgroundBrush = renderingProvider->CreateCustomBrush(Colors::White);
}

void ConfirmDialogOverlayControl::Initialize()
{
    ControlView::Initialize();

    AssertCriticalWinApiResult(SetLayeredWindowAttributes(windowHandle, 0, 175, LWA_ALPHA));
}

void ConfirmDialogOverlayControl::SetSize(Size size)
{
    AssertViewNotInitialized();
    layoutDescriptor = LayoutDescriptor::CreateFixedLayoutDescriptor(Point(0, 0), size);
}

Size ConfirmDialogOverlayControl::RenderContent(Renderer* renderer)
{
    renderer->DrawRect(Rect(Point(0, 0), viewStateDescriptor.GetSize()), backgroundBrush);
    return viewStateDescriptor.GetSize();
}

ConfirmDialogOverlayControl::~ConfirmDialogOverlayControl()
{
    delete backgroundBrush;
}