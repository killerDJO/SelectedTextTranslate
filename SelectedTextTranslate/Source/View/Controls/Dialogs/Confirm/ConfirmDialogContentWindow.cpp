#include "View\Framework\Windows\ChildWindow.h"
#include "View\Controls\Dialogs\Confirm\ConfirmDialogContentWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "View\Controls\Buttons\HoverFlatButtonWindow.h"
#include "View\Controls\Buttons\HoverTextButtonWindow.h"

ConfirmDialogContentWindow::ConfirmDialogContentWindow(WindowContext* context, Window* parentWindow)
    : ContentWindow(context, parentWindow)
{
    this->className = L"STT_CONFIRM_DIALOG_CONTENT";
    this->isLayered = true;
    this->paddingX = 10;
    this->paddingY = 5;
    this->lineHeight = 25;
}

void ConfirmDialogContentWindow::Initialize()
{
    ChildWindow::Initialize();
    AssertCriticalWinApiResult(SetWindowPos(windowHandle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE));
}

void ConfirmDialogContentWindow::SetText(wstring text)
{
    this->text = text;
}

wstring ConfirmDialogContentWindow::GetText() const
{
    return text;
}

Size ConfirmDialogContentWindow::RenderContent(Renderer* renderer)
{
    DestroyChildWindows();

    renderer->DrawBorderedRect(Rect(Point(0, 0), GetSize(true)), nullptr, 1, Colors::Gray);

    HFONT headerFont = context->GetRenderingContext()->CreateCustomFont(FontSizes::Normal, false, false, true);
    int headerFontAscent = renderer->GetFontAscent(headerFont);
    renderer->PrintText(L"Confirm settings reset", headerFont, Colors::Black, Point(paddingX, paddingY + headerFontAscent));
    renderer->DrawRect(Rect(Point(0, lineHeight), Size(GetSize(true).Width, 1)), context->GetRenderingContext()->CreateCustomBrush(Colors::Gray));

    int fontAscent = renderer->GetFontAscent(fontSmall);
    renderer->PrintText(L"Do you want to perform this action?", fontSmall, Colors::Black, Point(paddingX, lineHeight + fontAscent + 10));
    renderer->DrawBorderedRect(
        Rect(Rect(Point(0, lineHeight * 2.5), Size(GetSize(true).Width, GetSize(true).Height - lineHeight * 2.5))),
        context->GetRenderingContext()->CreateCustomBrush(Colors::Background),
        1,
        Colors::Gray);

    HoverFlatButtonWindow* confirmButton = new HoverFlatButtonWindow(context, this);
    confirmButton->SetText(L"Confirm");
    confirmButton->SetDimensions(Point(windowSize.Width - paddingX - 50, lineHeight * 2.5 + paddingX), Size(50, 21));
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

    return windowSize;
}

ConfirmDialogContentWindow::~ConfirmDialogContentWindow()
{
}
