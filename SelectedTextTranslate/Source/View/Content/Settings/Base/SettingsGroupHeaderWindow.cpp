#include "View\Content\Settings\Base\SettingsGroupHeaderWindow.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "View\Controls\Buttons\HoverIconButtonWindow.h"

SettingsGroupHeaderWindow::SettingsGroupHeaderWindow(WindowContext* context, Window* parentWindow)
    : ContentWindow(context, parentWindow)
{
    this->contentState = SettingsGroupContentState::Default;
    this->visibilityState = SettingsGroupVisibilityState::Collapsed;
    this->title = wstring();
    this->paddingX = this->paddingY = context->GetScaleProvider()->Scale(5);
    this->borderWidth = context->GetScaleProvider()->Scale(1);
    this->lineHeight = this->fontNormal->GetHeight();
    this->className = L"STT_SETTINGS_GROUP_HEADER";
}

void SettingsGroupHeaderWindow::SetDescriptor(WindowDescriptor descriptor)
{
    throw new SelectedTextTranslateFatalException(L"SetDescriptor is unsupported");
}

void SettingsGroupHeaderWindow::SetDimensions(Point position, int width)
{
    AssertWindowNotInitialized();
    this->position = position;
    this->windowSize = Size(width, 0);
}

void SettingsGroupHeaderWindow::SetTitle(wstring title)
{
    this->title = title;
}

wstring SettingsGroupHeaderWindow::GetTitle() const
{
    return title;
}

void SettingsGroupHeaderWindow::SetContentState(SettingsGroupContentState contentState)
{
    this->contentState = contentState;
}

SettingsGroupContentState SettingsGroupHeaderWindow::GetContentState() const
{
    return contentState;
}

void SettingsGroupHeaderWindow::SetVisibilityState(SettingsGroupVisibilityState visibilityState)
{
    this->visibilityState = visibilityState;
}

SettingsGroupVisibilityState SettingsGroupHeaderWindow::GetVisibilityState() const
{
    return visibilityState;
}

void SettingsGroupHeaderWindow::Initialize()
{
    int headerHeight = lineHeight + paddingY * 2;
    descriptor = WindowDescriptor::CreateFixedWindowDescriptor(position, Size(windowSize.GetWidth(), headerHeight));
    ContentWindow::Initialize();
}

void SettingsGroupHeaderWindow::SpecifyWindowClass(WNDCLASSEX* windowClass)
{
    windowClass->hCursor = LoadCursor(nullptr, IDC_HAND);
}

Size SettingsGroupHeaderWindow::RenderContent(Renderer* renderer)
{
    DestroyChildWindows();

    renderer->DrawBorderedRect(Rect(Point(0, 0), GetSize()), backgroundBrush, borderWidth, Colors::Gray);

    RenderPosition renderPosition = RenderPosition(paddingX, paddingY + fontNormal->GetAscent());

    renderPosition = renderer->PrintText(title, fontNormal, Colors::Black, renderPosition);
    if(contentState == SettingsGroupContentState::Modified)
    {
        renderPosition = renderer->PrintText(L"*", fontNormal, Colors::Black, renderPosition);
    }
    else if(contentState == SettingsGroupContentState::Invalid)
    {
        renderPosition = renderer->PrintText(L"*", fontNormal, Colors::Red, renderPosition);
    }

    int iconSize = fontNormal->GetAscent();

    HoverIconButtonWindow* expandButton = new HoverIconButtonWindow(context, this);
    expandButton->SetDimensions(
        Point(GetSize().GetWidth() - iconSize - paddingX, renderPosition.GetY() - iconSize + context->GetScaleProvider()->Scale(2)),
        Size(iconSize, iconSize));
    expandButton->SetNormalIconResource(visibilityState == SettingsGroupVisibilityState::Collapsed ? IDR_EXPAND_INACTIVE : IDR_COLLAPSE_INACTIVE);
    expandButton->SetHoverIconResource(visibilityState == SettingsGroupVisibilityState::Collapsed ? IDR_EXPAND : IDR_COLLAPSE);
    expandButton->SetBackgroundBrush(backgroundBrush);
    expandButton->OnClick.Subscribe(&OnSettingsToggled);
    AddChildWindow(expandButton);

    return renderer->GetSize();
}

LRESULT SettingsGroupHeaderWindow::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_LBUTTONUP)
    {
        OnSettingsToggled.Notify();
        return TRUE;
    }

    return ContentWindow::WindowProcedure(message, wParam, lParam);
}

SettingsGroupHeaderWindow::~SettingsGroupHeaderWindow()
{
}
