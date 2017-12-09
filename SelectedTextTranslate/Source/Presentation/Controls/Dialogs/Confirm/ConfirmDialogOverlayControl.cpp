#include "Presentation\Controls\Dialogs\Confirm\ConfirmDialogOverlayControl.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

ConfirmDialogOverlayControl::ConfirmDialogOverlayControl(ViewContext* context, View* parentWindow)
    : ControlView(context, parentWindow)
{
    this->IsLayered = true;
    this->backgroundBrush = RenderingProvider->CreateCustomBrush(Colors::White);
}

void ConfirmDialogOverlayControl::Initialize()
{
    ControlView::Initialize();

    AssertCriticalWinApiResult(SetLayeredWindowAttributes(Window->GetHandle(), 0, 175, LWA_ALPHA));
}

void ConfirmDialogOverlayControl::SetSize(Size size)
{
    AssertViewNotInitialized();
    ViewState->SetLayout(LayoutDescriptor::CreateFixedLayoutDescriptor(Point(0, 0), size));
}

void ConfirmDialogOverlayControl::RenderContent(Renderer* renderer)
{
    renderer->DrawRect(Rect(Point(0, 0), ViewState->GetViewSize()), backgroundBrush);
}

void ConfirmDialogOverlayControl::SpecifyWindow(NativeWindowHolder* window)
{
    window->SetClassName(L"STT_CONFIRM_DIALOG_OVERLAY");
}

ConfirmDialogOverlayControl::~ConfirmDialogOverlayControl()
{
    delete backgroundBrush;
}
