#include "View\Content\Settings\Base\SettingsGroupHeaderControl.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "View\Controls\Buttons\HoverIconButtonWindow.h"

SettingsGroupHeaderControl::SettingsGroupHeaderControl(ViewContext* context, View* parentWindow)
    : ContentView(context, parentWindow)
{
    this->contentState = SettingsGroupContentState::Default;
    this->visibilityState = SettingsGroupVisibilityState::Collapsed;
    this->title = wstring();
    this->paddingX = this->paddingY = scaleProvider->Scale(5);
    this->borderWidth = scaleProvider->Scale(1);
    this->lineHeight = this->fontNormal->GetHeight();
    this->className = L"STT_SETTINGS_GROUP_HEADER";
    this->viewName = L"SettingsGroupHeaderControl";
}

void SettingsGroupHeaderControl::SetDescriptor(WindowDescriptor descriptor)
{
    throw new SelectedTextTranslateFatalException(L"SetDescriptor is unsupported");
}

void SettingsGroupHeaderControl::SetDimensions(Point position, int width)
{
    AssertViewNotInitialized();
    descriptor = WindowDescriptor::CreateFixedWindowDescriptor(position, Size(width, 0));
}

void SettingsGroupHeaderControl::SetTitle(wstring title)
{
    this->title = title;
}

wstring SettingsGroupHeaderControl::GetTitle() const
{
    return title;
}

void SettingsGroupHeaderControl::SetContentState(SettingsGroupContentState contentState)
{
    this->contentState = contentState;
}

SettingsGroupContentState SettingsGroupHeaderControl::GetContentState() const
{
    return contentState;
}

void SettingsGroupHeaderControl::SetVisibilityState(SettingsGroupVisibilityState visibilityState)
{
    this->visibilityState = visibilityState;
}

SettingsGroupVisibilityState SettingsGroupHeaderControl::GetVisibilityState() const
{
    return visibilityState;
}

void SettingsGroupHeaderControl::Initialize()
{
    int headerHeight = lineHeight + paddingY * 2;
    descriptor = WindowDescriptor::CreateFixedWindowDescriptor(descriptor.GetPosition(), Size(descriptor.GetSize().GetWidth(), headerHeight));
    ContentView::Initialize();
}

Size SettingsGroupHeaderControl::RenderContent(Renderer* renderer)
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
        Point(GetSize().GetWidth() - iconSize - paddingX, renderPosition.MoveY(-iconSize).MoveY(2, scaleProvider).GetY()),
        Size(iconSize, iconSize));
    expandButton->SetNormalIconResource(visibilityState == SettingsGroupVisibilityState::Collapsed ? IDR_EXPAND_INACTIVE : IDR_COLLAPSE_INACTIVE);
    expandButton->SetHoverIconResource(visibilityState == SettingsGroupVisibilityState::Collapsed ? IDR_EXPAND : IDR_COLLAPSE);
    expandButton->SetBackgroundBrush(backgroundBrush);
    expandButton->OnClick.Subscribe(&OnSettingsToggled);
    expandButton->InitializeAndRender();

    return renderer->GetSize();
}

LRESULT SettingsGroupHeaderControl::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_LBUTTONUP)
    {
        OnSettingsToggled.Notify();
        return TRUE;
    }

    return ContentView::WindowProcedure(message, wParam, lParam);
}