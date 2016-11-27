#include "View\Content\Settings\Base\SettingsGroupHeaderWindow.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "View\Controls\Buttons\HoverIconButtonWindow.h"

SettingsGroupHeaderWindow::SettingsGroupHeaderWindow(WindowContext* context, Window* parentWindow)
    : ContentWindow(context, parentWindow)
{
    this->state = SettingsGroupState::Collapsed;
    this->title = wstring();
    this->paddingX = this->paddingY = 5;
    this->lineHeight = context->GetScaleProvider()->Downscale(context->GetRenderingContext()->GetFontMetrics(this->fontNormal).tmHeight);
    this->backgroundBrush = context->GetRenderingContext()->CreateCustomBrush(Colors::Background);
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

void SettingsGroupHeaderWindow::SetState(SettingsGroupState state)
{
    AssertWindowNotInitialized();
    this->state = state;
}

SettingsGroupState SettingsGroupHeaderWindow::GetState() const
{
    return state;
}

void SettingsGroupHeaderWindow::Initialize()
{
    descriptor = WindowDescriptor::CreateFixedWindowDescriptor(position, Size(windowSize.Width, lineHeight + paddingY * 2));
    ContentWindow::Initialize();
}

void SettingsGroupHeaderWindow::SpecifyWindowClass(WNDCLASSEX* windowClass)
{
    windowClass->hCursor = LoadCursor(nullptr, IDC_HAND);
}

Size SettingsGroupHeaderWindow::RenderContent(Renderer* renderer)
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

    return renderer->GetScaledSize();
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
    AssertCriticalWinApiResult(DeleteObject(backgroundBrush));
}
