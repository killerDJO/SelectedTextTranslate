#include "View\Content\Settings\Base\SettingsGroupWindow.h"
#include "View\Controls\Buttons\HoverIconButtonWindow.h"

SettingGroupWindow::SettingGroupWindow(WindowContext* context,
    Point position,
    int width,
    wstring name,
    Window* parentWindow,
    bool isCollapsed,
    wstring title,
    int lineHeight,
    int padding)
    : ContentWindow(
        context,
        isCollapsed 
            ? WindowDescriptor::CreateFixedWindowDescriptor(position, Size(width, lineHeight + padding * 2))
            : WindowDescriptor::CreateWindowDescriptor(position, Size(width, 0), OverflowModes::Fixed, OverflowModes::Stretch),
        name,
        parentWindow)
{
    this->isCollapsed = isCollapsed;
    this->title = title;
    this->padding = padding;
    this->lineHeight = lineHeight;
}

void SettingGroupWindow::Initialize()
{
    ContentWindow::Initialize();
}

Size SettingGroupWindow::RenderContent(Renderer* renderer)
{
    DestroyChildWindows();

    int windowWidth = context->GetScaleProvider()->Downscale(windowSize.Width);
    int headerHeight = lineHeight + padding * 2;

    Rect headerBorderRect = Rect(
        Point(0, 0),
        Size(windowWidth, headerHeight));
    renderer->DrawBorderedRect(
        headerBorderRect,
        context->GetRenderingContext()->CreateCustomBrush(Colors::Background),
        1,
        Colors::Gray);

    int fontAscent = renderer->GetFontAscent(fontNormal);
    int curY = padding + fontAscent;
    renderer->PrintText(title, fontNormal, Colors::Black, Point(padding, curY));

    int iconHeight = fontAscent;
    int iconWidth = iconHeight * 4 / 3;
    
    HoverIconButtonWindow* expandButton = new HoverIconButtonWindow(
        context,
        WindowDescriptor::CreateFixedWindowDescriptor(Point(windowWidth - 5 - iconWidth, curY - iconHeight + 2), Size(iconWidth, iconHeight)),
        L"ExpandButton",
        this,
        IDR_EXPAND_INACTIVE,
        IDR_EXPAND,
        context->GetRenderingContext()->CreateCustomBrush(Colors::Background));
    AddChildWindow(expandButton);

    if(!isCollapsed)
    {
        RenderSettingsContent(renderer, Point(paddingX, curY + paddingY));
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