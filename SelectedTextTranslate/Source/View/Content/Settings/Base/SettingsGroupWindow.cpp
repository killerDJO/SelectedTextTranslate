#include "View\Content\Settings\Base\SettingsGroupWindow.h"
#include "View\Controls\Buttons\HoverIconButtonWindow.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "View\Content\Settings\Base\SettingsGroupHeaderWindow.h"

SettingsGroupWindow::SettingsGroupWindow(WindowContext* context, Window* parentWindow)
    : ContentWindow(context, parentWindow)
{
    this->state = SettingsGroupState::Collapsed;
    this->title = wstring();
    this->paddingX = this->paddingY = 5;
    this->className = L"STT_SETTINGS_GROUP";
    this->headerWindow = nullptr;
    this->isModified = false;
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

void SettingsGroupWindow::SetModifiedState()
{
    isModified = true;
    UpdateModificationState();
}

void SettingsGroupWindow::SetNotModifiedState()
{
    isModified = false;
    UpdateModificationState();
}

void SettingsGroupWindow::UpdateModificationState() const
{
    if (windowState != WindowStates::New)
    {
        headerWindow->SetTitle(GetCurrentTitle());
        headerWindow->Render();
    }
}

wstring SettingsGroupWindow::GetCurrentTitle() const
{
    return title + (IsModified() ? L"*" : L"");
}

bool SettingsGroupWindow::IsModified() const
{
    return isModified;
}

void SettingsGroupWindow::Initialize()
{
    descriptor = WindowDescriptor::CreateWindowDescriptor(position, Size(windowSize.Width, 0), OverflowModes::Fixed, OverflowModes::Stretch);
    ContentWindow::Initialize();
}

Size SettingsGroupWindow::RenderContent(Renderer* renderer)
{
    DestroyChildWindows();

    headerWindow = new SettingsGroupHeaderWindow(context, this);
    headerWindow->SetTitle(GetCurrentTitle());
    headerWindow->SetState(state);
    headerWindow->SetDimensions(Point(0, 0), GetSize().Width);
    headerWindow->OnSettingsToggled.Subscribe(&OnSettingsToggled);
    AddChildWindow(headerWindow);
    headerWindow->Render();

    renderer->UpdateRenderedContentSize(headerWindow);

    if (state == SettingsGroupState::Expanded)
    {
        RenderSettingsContent(RenderDescriptor(renderer, Point(paddingX * 2, headerWindow->GetBoundingRect().GetBottom())));
        Rect contentBorderRect = Rect(Point(0, 0), Size(GetSize().Width, renderer->GetSize().Height));
        renderer->DrawBorderedRect(contentBorderRect, nullptr, 1, Colors::Gray);
    }

    return renderer->GetScaledSize();
}

SettingsGroupWindow::~SettingsGroupWindow()
{
}