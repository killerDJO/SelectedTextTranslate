#include "View\Content\Settings\Base\SettingsGroupHeaderWindow.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "View\Controls\Buttons\HoverIconButtonWindow.h"

SettingsGroupHeaderWindow::SettingsGroupHeaderWindow(ViewContext* context, View* parentWindow)
    : ContentView(context, parentWindow)
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
    AssertViewNotInitialized();
    descriptor = WindowDescriptor::CreateFixedWindowDescriptor(position, Size(width, 0));
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
    descriptor = WindowDescriptor::CreateFixedWindowDescriptor(descriptor.GetPosition(), Size(descriptor.GetSize().GetWidth(), headerHeight));
    ContentView::Initialize();
}

Size SettingsGroupHeaderWindow::RenderContent(Renderer* renderer)
{
    DestroyChildViews();

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
        Point(GetSize().GetWidth() - iconSize - paddingX, renderPosition.MoveY(-iconSize).MoveY(2, context->GetScaleProvider()).GetY()),
        Size(iconSize, iconSize));
    expandButton->SetNormalIconResource(visibilityState == SettingsGroupVisibilityState::Collapsed ? IDR_EXPAND_INACTIVE : IDR_COLLAPSE_INACTIVE);
    expandButton->SetHoverIconResource(visibilityState == SettingsGroupVisibilityState::Collapsed ? IDR_EXPAND : IDR_COLLAPSE);
    expandButton->SetBackgroundBrush(backgroundBrush);
    expandButton->OnClick.Subscribe(&OnSettingsToggled);
    expandButton->InitializeAndRender();

    return renderer->GetSize();
}

LRESULT SettingsGroupHeaderWindow::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_LBUTTONUP)
    {
        OnSettingsToggled.Notify();
        return TRUE;
    }

    return ContentView::WindowProcedure(message, wParam, lParam);
}