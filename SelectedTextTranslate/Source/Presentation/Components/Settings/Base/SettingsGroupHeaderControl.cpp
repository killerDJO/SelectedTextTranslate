#include "Presentation\Components\Settings\Base\SettingsGroupHeaderControl.h"
#include "Presentation\Controls\Buttons\HoverIconButtonControl.h"

SettingsGroupHeaderControl::SettingsGroupHeaderControl(ViewContext* context, View* parentWindow)
    : ControlView(context, parentWindow)
{
    this->contentState = SettingsGroupContentState::Default;
    this->visibilityState = SettingsGroupVisibilityState::Collapsed;
    this->title = wstring();

    this->fontNormal = RenderingProvider->CreateCustomFont(FontSizes::Normal);
    this->backgroundBrush = RenderingProvider->CreateCustomBrush(Colors::Background);
    this->paddingX = this->paddingY = ScaleProvider->Scale(5);
    this->borderWidth = ScaleProvider->Scale(1);
    this->lineHeight = this->fontNormal->GetHeight();

    this->ClassName = L"STT_SETTINGS_GROUP_HEADER";
    this->Name = L"SettingsGroupHeaderControl";
}

SettingsGroupHeaderControl* SettingsGroupHeaderControl::SetDimensions(Point position, int width)
{
    AssertViewNotInitialized();
    Layout = LayoutDescriptor::CreateFixedLayoutDescriptor(position, Size(width, 0));
    return this;
}

SettingsGroupHeaderControl* SettingsGroupHeaderControl::SetTitle(wstring title)
{
    this->title = title;
    return this;
}

wstring SettingsGroupHeaderControl::GetTitle() const
{
    return title;
}

SettingsGroupHeaderControl* SettingsGroupHeaderControl::SetContentState(SettingsGroupContentState contentState)
{
    this->contentState = contentState;
    return this;
}

SettingsGroupContentState SettingsGroupHeaderControl::GetContentState() const
{
    return contentState;
}

SettingsGroupHeaderControl* SettingsGroupHeaderControl::SetVisibilityState(SettingsGroupVisibilityState visibilityState)
{
    this->visibilityState = visibilityState;
    return this;
}

SettingsGroupVisibilityState SettingsGroupHeaderControl::GetVisibilityState() const
{
    return visibilityState;
}

void SettingsGroupHeaderControl::Initialize()
{
    int headerHeight = lineHeight + paddingY * 2;
    Layout = LayoutDescriptor::CreateFixedLayoutDescriptor(Layout.GetPosition(), Size(Layout.GetSize().GetWidth(), headerHeight));
    ControlView::Initialize();
}

Size SettingsGroupHeaderControl::RenderContent(Renderer* renderer)
{
    DestroyChildViews();

    renderer->DrawBorderedRect(Rect(Point(0, 0), Layout.GetSize()), backgroundBrush, borderWidth, Colors::Gray);

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

    HoverIconButtonControl* expandButton = new HoverIconButtonControl(Context, this);
    expandButton->SetDimensions(
        Point(Layout.GetSize().GetWidth() - iconSize - paddingX, renderPosition.MoveY(-iconSize).MoveY(2, ScaleProvider).GetY()),
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
        OnSettingsToggled();
        return TRUE;
    }

    return ControlView::WindowProcedure(message, wParam, lParam);
}

SettingsGroupHeaderControl::~SettingsGroupHeaderControl()
{
    delete backgroundBrush;
    delete fontNormal;
}