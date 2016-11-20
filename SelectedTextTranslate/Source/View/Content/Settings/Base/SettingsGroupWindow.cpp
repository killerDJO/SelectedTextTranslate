#include "View\Content\Settings\Base\SettingsGroupWindow.h"
#include "View\Controls\Buttons\HoverIconButtonWindow.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"

SettingGroupWindow::SettingGroupWindow(WindowContext* context, Window* parentWindow)
    : ContentWindow(context, parentWindow)
{
    this->isCollapsed = true;
    this->title = wstring();
    this->paddingX = this->paddingY = 5;
    this->lineHeight = context->GetScaleProvider()->Downscale(context->GetRenderingContext()->GetFontMetrics(this->fontNormal).tmHeight);
}

void SettingGroupWindow::SetDescriptor(WindowDescriptor descriptor)
{
    throw new SelectedTextTranslateFatalException(L"SetDescriptor is unsupported");
}

void SettingGroupWindow::SetDimensions(Point position, int width)
{
    AssertWindowNotInitialized();
    this->position = position;
    this->windowSize = Size(width, 0);
}

void SettingGroupWindow::SetTitle(wstring title)
{
    AssertWindowNotInitialized();
    this->title = title;
}

wstring SettingGroupWindow::GetTitle() const
{
    return title;
}

void SettingGroupWindow::SetState(bool isCollapsed)
{
    AssertWindowNotInitialized();
    this->isCollapsed = isCollapsed;
}

bool SettingGroupWindow::IsCollapsed() const
{
    return isCollapsed;
}

void SettingGroupWindow::Initialize()
{
    descriptor = isCollapsed
        ? WindowDescriptor::CreateFixedWindowDescriptor(position, Size(windowSize.Width, lineHeight + paddingY * 2))
        : WindowDescriptor::CreateWindowDescriptor(position, Size(windowSize.Width, 0), OverflowModes::Fixed, OverflowModes::Stretch);

    ContentWindow::Initialize();
}

Size SettingGroupWindow::RenderContent(Renderer* renderer)
{
    DestroyChildWindows();

    int windowWidth = context->GetScaleProvider()->Downscale(windowSize.Width);
    int headerHeight = lineHeight + paddingY * 2;

    Rect headerBorderRect = Rect(
        Point(0, 0),
        Size(windowWidth, headerHeight));
    renderer->DrawBorderedRect(
        headerBorderRect,
        context->GetRenderingContext()->CreateCustomBrush(Colors::Background),
        1,
        Colors::Gray);

    int fontAscent = renderer->GetFontAscent(fontNormal);
    int curY = paddingY + fontAscent;
    renderer->PrintText(title, fontNormal, Colors::Black, Point(paddingX, curY));

    int iconHeight = fontAscent;
    int iconWidth = iconHeight * 4 / 3;
    
    HoverIconButtonWindow* expandButton = new HoverIconButtonWindow(context, this);
    expandButton->SetDimensions(Point(windowWidth - 5 - iconWidth, curY - iconHeight + 2), Size(iconWidth, iconHeight));
    expandButton->SetNormalIconResource(IDR_EXPAND_INACTIVE);
    expandButton->SetHoverIconResource(IDR_EXPAND);
    expandButton->SetBackgroundBrush(context->GetRenderingContext()->CreateCustomBrush(Colors::Background));
    AddChildWindow(expandButton);

    if(!isCollapsed)
    {
        RenderSettingsContent(renderer, Point(paddingX, curY + paddingY * 2));
        Rect contentBorderRect = Rect(
            Point(0, headerHeight - 1),
            Size(windowWidth, renderer->GetSize().Height - headerHeight + 1));
        renderer->DrawBorderedRect(contentBorderRect, nullptr, 1, Colors::Gray);
    }

    return renderer->GetScaledSize();
}

SettingGroupWindow::~SettingGroupWindow()
{
}
