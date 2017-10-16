#include "View\Content\Settings\Base\SettingsGroupWindow.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "View\Controls\Buttons\HoverIconButtonWindow.h"
#include "View\Content\Settings\Base\SettingsGroupHeaderWindow.h"

SettingsGroupWindow::SettingsGroupWindow(ViewContext* context, View* parentWindow)
    : ContentView(context, parentWindow)
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
    AssertViewNotInitialized();
    descriptor = WindowDescriptor::CreateWindowDescriptor(position, Size(width, 0), OverflowModes::Fixed, OverflowModes::Stretch);
}

void SettingsGroupWindow::SetTitle(wstring title)
{
    this->title = title;
}

wstring SettingsGroupWindow::GetTitle() const
{
    return title;
}

void SettingsGroupWindow::SetSettings(Settings currentSettings, Settings globalSettings)
{
    this->currentSettings = currentSettings;
    this->globalSettings = globalSettings;
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
    if (viewState != ViewStates::New)
    {
        headerWindow->SetContentState(contentState);
        headerWindow->SetVisibilityState(visibilityState);
        headerWindow->Render();
    }
}

bool SettingsGroupWindow::IsValid() const
{
    return contentState != SettingsGroupContentState::Invalid;
}

Size SettingsGroupWindow::RenderContent(Renderer* renderer)
{
    DestroyChildViews();

    headerWindow = new SettingsGroupHeaderWindow(context, this);
    headerWindow->SetTitle(title);
    headerWindow->SetContentState(contentState);
    headerWindow->SetVisibilityState(visibilityState);
    headerWindow->SetDimensions(Point(0, 0), GetSize().GetWidth());
    headerWindow->OnSettingsToggled.Subscribe(&OnSettingsToggled);
    headerWindow->InitializeAndRender();

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