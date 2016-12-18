#include "View\Framework\Windows\ChildWindow.h"
#include "View\Controls\Dialogs\Confirm\ConfirmDialogContentWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "View\Controls\Buttons\HoverFlatButtonWindow.h"
#include "View\Controls\Buttons\HoverTextButtonWindow.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"

ConfirmDialogContentWindow::ConfirmDialogContentWindow(WindowContext* context, Window* parentWindow)
    : ContentWindow(context, parentWindow)
{
    this->className = L"STT_CONFIRM_DIALOG_CONTENT";
    this->isLayered = true;
    this->paddingX = 10;
    this->paddingY = 5;
    this->lineHeight = 25;

    this->headerFont = context->GetRenderingContext()->CreateCustomFont(FontSizes::Normal, false, false, true);

    this->OnConfirm = Subscribeable<>();
    this->OnCancel = Subscribeable<>();
}

void ConfirmDialogContentWindow::SetDescriptor(WindowDescriptor descriptor)
{
    throw new SelectedTextTranslateFatalException(L"SetDescriptor is unsupported");
}

void ConfirmDialogContentWindow::SetDimensions(PointReal position, double width)
{
    AssertWindowNotInitialized();

    this->position = context->GetScaleProvider()->Scale(position);
    this->windowSize = Size(context->GetScaleProvider()->Scale(width), 0);
}

void ConfirmDialogContentWindow::SetTitle(wstring title)
{
    this->title = title;

    if (windowState != WindowStates::New)
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
    descriptor = WindowDescriptor::CreateFixedWindowDescriptor(position, Size(windowSize.GetWidth(), context->GetScaleProvider()->Scale(103)));
    ChildWindow::Initialize();
    AssertCriticalWinApiResult(SetWindowPos(windowHandle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE));
}

Size ConfirmDialogContentWindow::RenderContent(Renderer* renderer)
{
    DestroyChildWindows();

    SizeReal scaledWindowSize = GetDownscaledSize();

    renderer->DrawBorderedRect(RectReal(PointReal(0, 0), scaledWindowSize), nullptr, 1, Colors::Gray);

    RenderPosition renderPosition = RenderPosition(paddingX, paddingY);

    double headerFontAscent = renderer->GetFontAscent(headerFont);
    renderer->PrintText(title, headerFont, Colors::Black, renderPosition.MoveY(headerFontAscent));

    renderPosition = renderPosition.SetY(lineHeight).SetX(0);
    renderer->DrawRect(RectReal(renderPosition.GetPosition(), SizeReal(scaledWindowSize.GetWidth(), 1)), grayBrush);

    double fontAscent = renderer->GetFontAscent(fontSmall);
    renderPosition = renderPosition.SetY(lineHeight + fontAscent + 10).SetX(paddingX);
    renderer->PrintText(L"Do you want to perform this action?", fontSmall, Colors::Black, renderPosition);

    renderPosition = renderPosition.SetY(roundToInt(lineHeight * 2.5)).SetX(0);
    renderer->DrawBorderedRect(
        RectReal(renderPosition.GetPosition(), SizeReal(scaledWindowSize.GetWidth(), scaledWindowSize.GetHeight() - renderPosition.GetY())),
        backgroundBrush,
        1,
        Colors::Gray);

    HoverFlatButtonWindow* confirmButton = new HoverFlatButtonWindow(context, this);
    confirmButton->SetText(L"Confirm");
    confirmButton->SetPosition(PointReal(scaledWindowSize.GetWidth() - paddingX - 60, renderPosition.GetY() + paddingX));
    confirmButton->OnClick.Subscribe(&OnConfirm);
    confirmButton->EnableLayeredMode();
    AddChildWindow(confirmButton);

    double cancelButtonFontAscent = renderer->GetFontAscent(fontSmallUnderscored);
    int textWidth = context->GetRenderingContext()->GetTextSize(L"Cancel", fontSmallUnderscored).GetWidth();
    HoverTextButtonWindow* cancelButton = new HoverTextButtonWindow(context, this);
    cancelButton->SetText(L"Cancel");
    cancelButton->SetFont(fontSmallUnderscored);
    cancelButton->SetPosition(PointReal(confirmButton->GetDownscaledPosition().GetX() - 10 - textWidth, confirmButton->GetDownscaledPosition().GetY() + confirmButton->GetTextBaseline() - cancelButtonFontAscent));
    cancelButton->OnClick.Subscribe(&OnCancel);
    cancelButton->EnableLayeredMode();
    cancelButton->SetBackgroundColor(Colors::Background);
    AddChildWindow(cancelButton);
    cancelButton->Draw();

    return windowSize;
}

ConfirmDialogContentWindow::~ConfirmDialogContentWindow()
{
    context->GetRenderingContext()->DeleteCustomFont(headerFont);
}
