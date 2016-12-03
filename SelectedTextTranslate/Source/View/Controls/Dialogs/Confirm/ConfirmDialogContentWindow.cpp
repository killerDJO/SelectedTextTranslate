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
    descriptor = WindowDescriptor::CreateFixedWindowDescriptor(position, Size(windowSize.Width, 103));
    ChildWindow::Initialize();
    AssertCriticalWinApiResult(SetWindowPos(windowHandle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE));
}

Size ConfirmDialogContentWindow::RenderContent(Renderer* renderer)
{
    DestroyChildWindows();
    
    Size scaledWindowSize = GetSize();

    renderer->DrawBorderedRect(Rect(Point(0, 0), scaledWindowSize), nullptr, 1, Colors::Gray);

    RenderPosition renderPosition = RenderPosition(paddingX, paddingY);

    int headerFontAscent = renderer->GetFontAscent(headerFont);
    renderer->PrintText(title, headerFont, Colors::Black, renderPosition.MoveY(headerFontAscent));

    renderPosition = renderPosition.SetY(lineHeight).SetX(0);
    renderer->DrawRect(Rect(renderPosition.GetPosition(), Size(scaledWindowSize.Width, 1)), grayBrush);

    int fontAscent = renderer->GetFontAscent(fontSmall);
    renderPosition = renderPosition.SetY(lineHeight + fontAscent + 10).SetX(paddingX);
    renderer->PrintText(L"Do you want to perform this action?", fontSmall, Colors::Black, renderPosition);

    renderPosition = renderPosition.SetY(roundToInt(lineHeight * 2.5)).SetX(0);
    renderer->DrawBorderedRect(
        Rect(Rect(renderPosition.GetPosition(), Size(scaledWindowSize.Width, scaledWindowSize.Height - renderPosition.GetY()))),
        backgroundBrush,
        1,
        Colors::Gray);

    Size confirmButtonSize = Size(60, 21);
    HoverFlatButtonWindow* confirmButton = new HoverFlatButtonWindow(context, this);
    confirmButton->SetText(L"Confirm");
    confirmButton->SetDimensions(Point(scaledWindowSize.Width - paddingX - confirmButtonSize.Width, renderPosition.GetY() + paddingX), confirmButtonSize);
    confirmButton->OnClick.Subscribe(&OnConfirm);
    confirmButton->EnableLayeredMode();
    AddChildWindow(confirmButton);

    int cancelButtonFontAscent = renderer->GetFontAscent(fontSmallUnderscored);
    int textWidth = context->GetRenderingContext()->GetTextSize(L"Cancel", fontSmallUnderscored).Width;
    HoverTextButtonWindow* cancelButton = new HoverTextButtonWindow(context, this);
    cancelButton->SetText(L"Cancel");
    cancelButton->SetFont(fontSmallUnderscored);
    cancelButton->SetPosition(Point(confirmButton->GetPosition().X - 10 - textWidth, confirmButton->GetPosition().Y + confirmButton->GetTextBaseline() - cancelButtonFontAscent));
    cancelButton->OnClick.Subscribe(&OnCancel);
    cancelButton->EnableLayeredMode();
    cancelButton->SetBackgroundColor(Colors::Background);
    AddChildWindow(cancelButton);
    cancelButton->Draw();

    return windowSize;
}

ConfirmDialogContentWindow::~ConfirmDialogContentWindow()
{
    AssertCriticalWinApiResult(DeleteObject(headerFont));
}
