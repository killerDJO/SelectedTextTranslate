#include "View\Content\Settings\Base\SettingsGroupWindow.h"
#include "View\Controls\Buttons\HoverIconButtonWindow.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

SettingsGroupWindow::SettingsGroupWindow(WindowContext* context, Window* parentWindow)
    : ContentWindow(context, parentWindow)
{
    this->state = SettingsGroupState::Collapsed;
    this->title = wstring();
    this->paddingX = this->paddingY = 5;
    this->lineHeight = context->GetScaleProvider()->Downscale(context->GetRenderingContext()->GetFontMetrics(this->fontNormal).tmHeight);
    this->backgroundBrush = context->GetRenderingContext()->CreateCustomBrush(Colors::Background);
    this->className = L"STT_SETTINGS";
}

void SettingsGroupWindow::SetDescriptor(WindowDescriptor descriptor)
{
    throw new SelectedTextTranslateFatalException(L"SetDescriptor is unsupported");
}

void SettingsGroupWindow::SetDimensions(Point position, int width)
{
    AssertWindowNotInitialized();
    this->position = position;
    this->windowSize = Size(width, 0);
}

void SettingsGroupWindow::SetTitle(wstring title)
{
    AssertWindowNotInitialized();
    this->title = title;
}

wstring SettingsGroupWindow::GetTitle() const
{
    return title;
}

void SettingsGroupWindow::SetState(SettingsGroupState state)
{
    AssertWindowNotInitialized();
    this->state = state;
}

SettingsGroupState SettingsGroupWindow::GetState() const
{
    return state;
}

void SettingsGroupWindow::Initialize()
{
    descriptor = state == SettingsGroupState::Collapsed
        ? WindowDescriptor::CreateFixedWindowDescriptor(position, Size(windowSize.Width, lineHeight + paddingY * 2))
        : WindowDescriptor::CreateWindowDescriptor(position, Size(windowSize.Width, 0), OverflowModes::Fixed, OverflowModes::Stretch);

    ContentWindow::Initialize();
}

Size SettingsGroupWindow::RenderContent(Renderer* renderer)
{
    DestroyChildWindows();

    int windowWidth = GetSize(true).Width;
    int headerHeight = lineHeight + paddingY * 2;

    renderer->DrawBorderedRect(Rect(Point(0, 0), Size(windowWidth, headerHeight)), backgroundBrush, 1, Colors::Gray);

    int fontAscent = renderer->GetFontAscent(fontNormal);
    int curY = paddingY + fontAscent;
    renderer->PrintText(title, fontNormal, Colors::Black, Point(paddingX, curY));

    int iconSize = fontAscent;

    HoverIconButtonWindow* expandButton = new HoverIconButtonWindow(context, this);
    expandButton->SetDimensions(Point(windowWidth - iconSize - paddingX, curY - iconSize + 2), Size(iconSize, iconSize));
    expandButton->SetNormalIconResource(state == SettingsGroupState::Collapsed ? IDR_EXPAND_INACTIVE : IDR_COLLAPSE_INACTIVE);
    expandButton->SetHoverIconResource(state == SettingsGroupState::Collapsed ? IDR_EXPAND : IDR_COLLAPSE);
    expandButton->SetBackgroundBrush(backgroundBrush);
    expandButton->OnClick.Subscribe(&OnSettingsToggled);
    AddChildWindow(expandButton);

    if (state == SettingsGroupState::Expanded)
    {
        RenderSettingsContent(renderer, Point(paddingX * 2, curY + paddingY * 2));
        Rect contentBorderRect = Rect(Point(0, 0), Size(windowWidth, renderer->GetSize().Height));
        renderer->DrawBorderedRect(contentBorderRect, nullptr, 1, Colors::Gray);
    }

    return renderer->GetScaledSize();
}

SettingsGroupWindow::~SettingsGroupWindow()
{
    AssertCriticalWinApiResult(DeleteObject(backgroundBrush));
}
