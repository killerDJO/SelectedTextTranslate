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

void ConfirmDialogContentWindow::SetDimensions(Point position, int width)
{
    AssertWindowNotInitialized();

    this->position = position;
    this->windowSize = Size(width, 0);
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

    SizeReal dowscaledWindowSize = context->GetScaleProvider()->Downscale(GetSize());

    renderer->DrawBorderedRect(RectReal(PointReal(0, 0), dowscaledWindowSize), nullptr, 1, Colors::Gray);

    RenderPosition renderPosition = RenderPosition(paddingX, paddingY);

    double headerFontAscent = renderer->GetFontAscent(headerFont);
    renderer->PrintText(title, headerFont, Colors::Black, renderPosition.MoveY(headerFontAscent));

    renderPosition = renderPosition.SetY(lineHeight).SetX(0);
    renderer->DrawRect(RectReal(renderPosition.GetPosition(), SizeReal(dowscaledWindowSize.GetWidth(), 1)), grayBrush);

    double fontAscent = renderer->GetFontAscent(fontSmall);
    renderPosition = renderPosition.SetY(lineHeight + fontAscent + 10).SetX(paddingX);
    renderer->PrintText(L"Do you want to perform this action?", fontSmall, Colors::Black, renderPosition);

    renderPosition = renderPosition.SetY(roundToInt(lineHeight * 2.5)).SetX(0);
    renderer->DrawBorderedRect(
        RectReal(renderPosition.GetPosition(), SizeReal(dowscaledWindowSize.GetWidth(), dowscaledWindowSize.GetHeight() - renderPosition.GetY())),
        backgroundBrush,
        1,
        Colors::Gray);

    HoverFlatButtonWindow* confirmButton = new HoverFlatButtonWindow(context, this);
    confirmButton->SetText(L"Confirm");
    confirmButton->SetPosition(context->GetScaleProvider()->Scale(PointReal(
        dowscaledWindowSize.GetWidth() - paddingX - context->GetScaleProvider()->Downscale(confirmButton->GetComputedSize().GetWidth()),
        renderPosition.GetY() + paddingX)));
    confirmButton->OnClick.Subscribe(&OnConfirm);
    confirmButton->EnableLayeredMode();
    AddChildWindow(confirmButton);

    PointReal downscaledConfirmButtonPosition = context->GetScaleProvider()->Downscale(confirmButton->GetPosition());
    double cancelButtonFontAscent = renderer->GetFontAscent(fontSmallUnderscored);
    double textWidth = context->GetScaleProvider()->Downscale(context->GetRenderingContext()->GetTextSize(L"Cancel", fontSmallUnderscored).GetWidth());
    HoverTextButtonWindow* cancelButton = new HoverTextButtonWindow(context, this);
    cancelButton->SetText(L"Cancel");
    cancelButton->SetFont(fontSmallUnderscored);
    cancelButton->SetPosition(context->GetScaleProvider()->Scale(PointReal(
        downscaledConfirmButtonPosition.GetX() - 10 - textWidth,
        downscaledConfirmButtonPosition.GetY() + confirmButton->GetTextBaseline() - cancelButtonFontAscent)));
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
