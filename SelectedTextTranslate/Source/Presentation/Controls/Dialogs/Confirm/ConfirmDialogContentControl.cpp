#include "Presentation\Controls\Dialogs\Confirm\ConfirmDialogContentControl.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Presentation\Framework\Views\ChildView.h"
#include "Presentation\Controls\Buttons\HoverFlatButtonControl.h"
#include "Presentation\Controls\Buttons\HoverTextButtonControl.h"

ConfirmDialogContentControl::ConfirmDialogContentControl(CommonContext* context, View* parentWindow)
    : ControlView(context, parentWindow)
{
    this->className = L"STT_CONFIRM_DIALOG_CONTENT";
    this->isLayered = true;
    this->paddingX = scaleProvider->Scale(10);
    this->paddingY = scaleProvider->Scale(5);
    this->lineHeight = scaleProvider->Scale(25);
    this->borderWidth = scaleProvider->Scale(1);
    this->height = scaleProvider->Scale(103);

    this->fontSmall = renderingProvider->CreateCustomFont(FontSizes::Small);
    this->fontSmallUnderscored = renderingProvider->CreateCustomFont(FontSizes::Small, false, true);

    this->grayBrush = renderingProvider->CreateCustomBrush(Colors::Gray);
    this->backgroundBrush = renderingProvider->CreateCustomBrush(Colors::Background);

    this->headerFont = renderingProvider->CreateCustomFont(FontSizes::Normal, false, false, true);

    this->OnConfirm = Subscribeable<>();
    this->OnCancel = Subscribeable<>();
}

void ConfirmDialogContentControl::SetDimensions(Point position, int width)
{
    AssertViewNotInitialized();

    layoutDescriptor = LayoutDescriptor::CreateFixedLayoutDescriptor(position, Size(width, height));
}

void ConfirmDialogContentControl::SetTitle(wstring title)
{
    this->title = title;

    if (viewStateDescriptor.GetViewState() != ViewStates::New)
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
    AssertCriticalWinApiResult(SetWindowPos(windowHandle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE));
}

Size ConfirmDialogContentControl::RenderContent(Renderer* renderer)
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

    HoverFlatButtonControl* confirmButton = new HoverFlatButtonControl(context, this);
    confirmButton->SetText(L"Confirm");
    confirmButton->SetPosition(Point(
        GetSize().GetWidth() - paddingX - confirmButton->GetComputedSize().GetWidth(),
        renderPosition.GetY() + paddingX));
    confirmButton->OnClick.Subscribe(&OnConfirm);
    confirmButton->EnableLayeredMode();
    confirmButton->InitializeAndRender();

    int textWidth = renderingProvider->GetTextSize(L"Cancel", fontSmallUnderscored).GetWidth();
    HoverTextButtonControl* cancelButton = new HoverTextButtonControl(context, this);
    cancelButton->SetText(L"Cancel");
    cancelButton->SetFont(fontSmallUnderscored);
    cancelButton->SetPosition(Point(
        confirmButton->GetPosition().GetX() - paddingX - textWidth,
        confirmButton->GetPosition().GetY() + confirmButton->GetTextBaseline() - fontSmallUnderscored->GetAscent()));
    cancelButton->OnClick.Subscribe(&OnCancel);
    cancelButton->EnableLayeredMode();
    cancelButton->SetBackgroundColor(Colors::Background);
    cancelButton->InitializeAndRender();

    return viewStateDescriptor.GetSize();
}

ConfirmDialogContentControl::~ConfirmDialogContentControl()
{
    delete headerFont;
    delete fontSmallUnderscored;
    delete fontSmall;
    delete grayBrush;
    delete backgroundBrush;
}
