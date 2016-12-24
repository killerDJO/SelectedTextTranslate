#include "View\Content\Settings\Base\SettingsGroupWindow.h"
#include "View\Controls\Buttons\HoverIconButtonWindow.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "View\Content\Settings\Base\SettingsGroupHeaderWindow.h"

SettingsGroupWindow::SettingsGroupWindow(WindowContext* context, Window* parentWindow)
    : ContentWindow(context, parentWindow)
{
    this->contentState = SettingsGroupContentState::Default;
    this->visibilityState = SettingsGroupVisibilityState::Collapsed;

    this->title = wstring();
    this->paddingX = this->paddingY = context->GetScaleProvider()->Scale(5);
    this->borderWidth = context->GetScaleProvider()->Scale(1);
    this->className = L"STT_SETTINGS_GROUP";
    this->headerWindow = nullptr;
}

void SettingsGroupWindow::SetDescriptor(WindowDescriptor descriptor)
{
    throw new SelectedTextTranslateFatalException(L"SetDescriptor is unsupported");
}

void SettingsGroupWindow::SetDimensions(Point position, int width)
{
    AssertWindowNotInitialized();
    this->position = position;
    this->currentWindowSize = Size(width, 0);
}

void SettingsGroupWindow::SetTitle(wstring title)
{
    this->title = title;
}

wstring SettingsGroupWindow::GetTitle() const
{
    return title;
}

SettingsGroupContentState SettingsGroupWindow::GetContentState() const
{
    return contentState;
}

void SettingsGroupWindow::SetVisibilityState(SettingsGroupVisibilityState visibilityState)
{
    this->visibilityState = visibilityState;
    UpdateModificationState();
}

SettingsGroupVisibilityState SettingsGroupWindow::GetVisibilityState() const
{
    return visibilityState;
}

void SettingsGroupWindow::UpdateModificationState() const
{
    if (windowState != WindowStates::New)
    {
        headerWindow->SetContentState(contentState);
        headerWindow->SetVisibilityState(visibilityState);
        headerWindow->Render();
    }
}

void SettingsGroupWindow::Initialize()
{
    descriptor = WindowDescriptor::CreateWindowDescriptor(position, currentWindowSize, OverflowModes::Fixed, OverflowModes::Stretch);
    ContentWindow::Initialize();
}

bool SettingsGroupWindow::IsValid() const
{
    return contentState != SettingsGroupContentState::Invalid;
}

Size SettingsGroupWindow::RenderContent(Renderer* renderer)
{
    DestroyChildWindows();

    headerWindow = new SettingsGroupHeaderWindow(context, this);
    headerWindow->SetTitle(title);
    headerWindow->SetContentState(contentState);
    headerWindow->SetVisibilityState(visibilityState);
    headerWindow->SetDimensions(Point(0, 0), GetSize().GetWidth());
    headerWindow->OnSettingsToggled.Subscribe(&OnSettingsToggled);
    AddChildWindow(headerWindow);
    headerWindow->Render();

    renderer->UpdateRenderedContentSize(headerWindow);

    if (visibilityState == SettingsGroupVisibilityState::Expanded)
    {
        RenderSettingsContent(RenderDescriptor(renderer, Point(paddingX * 2, headerWindow->GetBoundingRect().GetBottom())));
        Rect contentBorderRect = Rect(Point(0, 0), Size(GetSize().GetWidth(), renderer->GetSize().GetHeight()));
        renderer->DrawBorderedRect(contentBorderRect, nullptr, borderWidth, Colors::Gray);
    }

    ComputeContentState();

    return renderer->GetSize();
}

SettingsGroupWindow::~SettingsGroupWindow()
{
}