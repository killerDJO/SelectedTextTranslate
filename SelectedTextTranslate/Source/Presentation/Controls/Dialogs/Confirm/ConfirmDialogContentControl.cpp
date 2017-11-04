#include "Presentation\Controls\Dialogs\Confirm\ConfirmDialogContentControl.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Presentation\Framework\Views\ChildView.h"
#include "Presentation\Controls\Buttons\HoverFlatButtonControl.h"
#include "Presentation\Controls\Buttons\HoverTextButtonControl.h"

ConfirmDialogContentControl::ConfirmDialogContentControl(ViewContext* context, View* parentWindow)
    : ControlView(context, parentWindow)
{
    this->ClassName = L"STT_CONFIRM_DIALOG_CONTENT";
    this->IsLayered = true;
    this->paddingX = ScaleProvider->Scale(10);
    this->paddingY = ScaleProvider->Scale(5);
    this->lineHeight = ScaleProvider->Scale(25);
    this->borderWidth = ScaleProvider->Scale(1);
    this->height = ScaleProvider->Scale(103);

    this->fontSmall = RenderingProvider->CreateCustomFont(FontSizes::Small);
    this->fontSmallUnderscored = RenderingProvider->CreateCustomFont(FontSizes::Small, false, true);

    this->grayBrush = RenderingProvider->CreateCustomBrush(Colors::Gray);
    this->backgroundBrush = RenderingProvider->CreateCustomBrush(Colors::Background);

    this->headerFont = RenderingProvider->CreateCustomFont(FontSizes::Normal, false, false, true);

    this->OnConfirm = Subscribable<>();
    this->OnCancel = Subscribable<>();
}

void ConfirmDialogContentControl::SetDimensions(Point position, int width)
{
    AssertViewNotInitialized();
    State->SetLayout(LayoutDescriptor::CreateFixedLayoutDescriptor(position, Size(width, height)));
}

void ConfirmDialogContentControl::SetTitle(wstring title)
{
    this->title = title;

    if (State->GetViewState() != ViewStates::New)
    {
        Render();
    }
}

wstring ConfirmDialogContentControl::GetTitle() const
{
    return title;
}

void ConfirmDialogContentControl::Initialize()
{
    ControlView::Initialize();
    AssertCriticalWinApiResult(SetWindowPos(Handle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE));
}

void ConfirmDialogContentControl::RenderContent(Renderer* renderer)
{
    DestroyChildViews();

    renderer->DrawBorderedRect(Rect(Point(0, 0), State->GetWindowSize()), nullptr, borderWidth, Colors::Gray);

    RenderPosition renderPosition = RenderPosition(paddingX, paddingY);

    renderer->PrintText(title, headerFont, Colors::Black, renderPosition.MoveY(headerFont->GetAscent()));

    renderPosition = renderPosition
        .SetY(lineHeight)
        .SetX(0);

    renderer->DrawRect(Rect(renderPosition.GetPosition(), Size(State->GetWindowSize().GetWidth(), borderWidth)), grayBrush);

    renderPosition = renderPosition
        .SetY(roundToInt(1.5 * lineHeight) + fontSmall->GetAscent())
        .SetX(paddingX);
    renderer->PrintText(L"Do you want to perform this action?", fontSmall, Colors::Black, renderPosition);

    renderPosition = renderPosition.SetY(roundToInt(lineHeight * 2.5)).SetX(0);
    renderer->DrawBorderedRect(
        Rect(renderPosition.GetPosition(), Size(State->GetWindowSize().GetWidth(), State->GetWindowSize().GetHeight() - renderPosition.GetY())),
        backgroundBrush,
        borderWidth,
        Colors::Gray);

    HoverFlatButtonControl* confirmButton = new HoverFlatButtonControl(Context, this);
    confirmButton->SetText(L"Confirm");
    confirmButton->SetPosition(Point(
        State->GetWindowSize().GetWidth() - paddingX - confirmButton->GetComputedSize().GetWidth(),
        renderPosition.GetY() + paddingX));
    confirmButton->OnClick.Subscribe(&OnConfirm);
    confirmButton->EnableLayeredMode();
    confirmButton->InitializeAndRender();

    int textWidth = RenderingProvider->GetTextSize(L"Cancel", fontSmallUnderscored).GetWidth();
    HoverTextButtonControl* cancelButton = new HoverTextButtonControl(Context, this);
    cancelButton->SetText(L"Cancel");
    cancelButton->SetFont(fontSmallUnderscored);
    cancelButton->SetPosition(Point(
        confirmButton->GetBoundingRect().GetX() - paddingX - textWidth,
        confirmButton->GetBoundingRect().GetY() + confirmButton->GetTextBaseline() - fontSmallUnderscored->GetAscent()));
    cancelButton->OnClick.Subscribe(&OnCancel);
    cancelButton->EnableLayeredMode();
    cancelButton->SetBackgroundColor(Colors::Background);
    cancelButton->InitializeAndRender();
}

ConfirmDialogContentControl::~ConfirmDialogContentControl()
{
    delete headerFont;
    delete fontSmallUnderscored;
    delete fontSmall;
    delete grayBrush;
    delete backgroundBrush;
}
