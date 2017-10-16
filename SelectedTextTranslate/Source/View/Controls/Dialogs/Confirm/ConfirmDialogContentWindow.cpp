#include "View\Controls\Dialogs\Confirm\ConfirmDialogContentWindow.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "View\Framework\View\Views\ChildView.h"
#include "View\Controls\Buttons\HoverFlatButtonWindow.h"
#include "View\Controls\Buttons\HoverTextButtonWindow.h"

ConfirmDialogContentWindow::ConfirmDialogContentWindow(ViewContext* context, View* parentWindow)
    : ContentView(context, parentWindow)
{
    this->className = L"STT_CONFIRM_DIALOG_CONTENT";
    this->isLayered = true;
    this->paddingX = context->GetScaleProvider()->Scale(10);
    this->paddingY = context->GetScaleProvider()->Scale(5);
    this->lineHeight = context->GetScaleProvider()->Scale(25);
    this->borderWidth = context->GetScaleProvider()->Scale(1);
    this->height = context->GetScaleProvider()->Scale(103);

    this->headerFont = context->GetRenderingProvider()->CreateCustomFont(FontSizes::Normal, false, false, true);

    this->OnConfirm = Subscribeable<>();
    this->OnCancel = Subscribeable<>();
}

void ConfirmDialogContentWindow::SetDescriptor(WindowDescriptor descriptor)
{
    throw new SelectedTextTranslateFatalException(L"SetDescriptor is unsupported");
}

void ConfirmDialogContentWindow::SetDimensions(Point position, int width)
{
    AssertViewNotInitialized();

    descriptor = WindowDescriptor::CreateFixedWindowDescriptor(position, Size(width, height));
}

void ConfirmDialogContentWindow::SetTitle(wstring title)
{
    this->title = title;

    if (viewState != ViewStates::New)
    {
        Render();
    }
}

wstring ConfirmDialogContentWindow::GetTitle() const
{
    return title;
}

void ConfirmDialogContentWindow::Initialize()
{
    ChildView::Initialize();
    AssertCriticalWinApiResult(SetWindowPos(windowHandle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE));
}

Size ConfirmDialogContentWindow::RenderContent(Renderer* renderer)
{
    DestroyChildViews();

    renderer->DrawBorderedRect(Rect(Point(0, 0), GetSize()), nullptr, borderWidth, Colors::Gray);

    RenderPosition renderPosition = RenderPosition(paddingX, paddingY);

    renderer->PrintText(title, headerFont, Colors::Black, renderPosition.MoveY(headerFont->GetAscent()));

    renderPosition = renderPosition
        .SetY(lineHeight)
        .SetX(0);

    renderer->DrawRect(Rect(renderPosition.GetPosition(), Size(GetSize().GetWidth(), borderWidth)), grayBrush);

    renderPosition = renderPosition
        .SetY(roundToInt(1.5 * lineHeight) + fontSmall->GetAscent())
        .SetX(paddingX);
    renderer->PrintText(L"Do you want to perform this action?", fontSmall, Colors::Black, renderPosition);

    renderPosition = renderPosition.SetY(roundToInt(lineHeight * 2.5)).SetX(0);
    renderer->DrawBorderedRect(
        Rect(renderPosition.GetPosition(), Size(GetSize().GetWidth(), GetSize().GetHeight() - renderPosition.GetY())),
        backgroundBrush,
        borderWidth,
        Colors::Gray);

    HoverFlatButtonWindow* confirmButton = new HoverFlatButtonWindow(context, this);
    confirmButton->SetText(L"Confirm");
    confirmButton->SetPosition(Point(
        GetSize().GetWidth() - paddingX - confirmButton->GetComputedSize().GetWidth(),
        renderPosition.GetY() + paddingX));
    confirmButton->OnClick.Subscribe(&OnConfirm);
    confirmButton->EnableLayeredMode();
    confirmButton->InitializeAndRender();

    int textWidth = context->GetRenderingProvider()->GetTextSize(L"Cancel", fontSmallUnderscored).GetWidth();
    HoverTextButtonWindow* cancelButton = new HoverTextButtonWindow(context, this);
    cancelButton->SetText(L"Cancel");
    cancelButton->SetFont(fontSmallUnderscored);
    cancelButton->SetPosition(Point(
        confirmButton->GetPosition().GetX() - paddingX - textWidth,
        confirmButton->GetPosition().GetY() + confirmButton->GetTextBaseline() - fontSmallUnderscored->GetAscent()));
    cancelButton->OnClick.Subscribe(&OnCancel);
    cancelButton->EnableLayeredMode();
    cancelButton->SetBackgroundColor(Colors::Background);
    cancelButton->InitializeAndRender();

    return nativeStateDescriptor.GetSize();
}

ConfirmDialogContentWindow::~ConfirmDialogContentWindow()
{
    delete headerFont;
}
