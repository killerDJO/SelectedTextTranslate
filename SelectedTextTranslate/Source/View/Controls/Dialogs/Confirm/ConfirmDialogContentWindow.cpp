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

    renderer->DrawBorderedRect(Rect(Point(0, 0), GetSize(true)), nullptr, 1, Colors::Gray);

    int delimiterY = lineHeight;

    HFONT headerFont = context->GetRenderingContext()->CreateCustomFont(FontSizes::Normal, false, false, true);
    int headerFontAscent = renderer->GetFontAscent(headerFont);
    renderer->PrintText(title, headerFont, Colors::Black, Point(paddingX, paddingY + headerFontAscent));
    renderer->DrawRect(Rect(Point(0, delimiterY), Size(GetSize(true).Width, 1)), context->GetRenderingContext()->CreateCustomBrush(Colors::Gray));

    delimiterY = roundToInt(delimiterY * 2.5);

    int fontAscent = renderer->GetFontAscent(fontSmall);
    renderer->PrintText(L"Do you want to perform this action?", fontSmall, Colors::Black, Point(paddingX, lineHeight + fontAscent + 10));
    renderer->DrawBorderedRect(
        Rect(Rect(Point(0, delimiterY), Size(GetSize(true).Width, GetSize(true).Height - delimiterY))),
        context->GetRenderingContext()->CreateCustomBrush(Colors::Background),
        1,
        Colors::Gray);

    Size confirmButtonSize = Size(60, 21);
    HoverFlatButtonWindow* confirmButton = new HoverFlatButtonWindow(context, this);
    confirmButton->SetText(L"Confirm");
    confirmButton->SetDimensions(Point(GetSize(true).Width - paddingX - confirmButtonSize.Width, delimiterY + paddingX), confirmButtonSize);
    confirmButton->OnClick.Subscribe(&OnConfirm);
    confirmButton->EnableLayeredMode();
    AddChildWindow(confirmButton);

    HFONT fontSmallUnderscored = context->GetRenderingContext()->CreateCustomFont(FontSizes::Small, false, true);
    int cancelButtonFontAscent = renderer->GetFontAscent(fontSmallUnderscored);
    int textWidth = context->GetRenderingContext()->GetTextSize(L"Cancel", fontSmallUnderscored).Width;
    HoverTextButtonWindow* cancelButton = new HoverTextButtonWindow(context, this);
    cancelButton->SetText(L"Cancel");
    cancelButton->SetFont(fontSmallUnderscored);
    cancelButton->SetPosition(Point(confirmButton->GetPosition(true).X - 10 - textWidth, confirmButton->GetPosition(true).Y + confirmButton->GetTextBaseline() - cancelButtonFontAscent));
    cancelButton->OnClick.Subscribe(&OnCancel);
    cancelButton->EnableLayeredMode();
    cancelButton->SetBackgroundColor(Colors::Background);
    AddChildWindow(cancelButton);
    cancelButton->Draw();

    AssertCriticalWinApiResult(SetWindowPos(windowHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE));

    return windowSize;
}

ConfirmDialogContentWindow::~ConfirmDialogContentWindow()
{
}
