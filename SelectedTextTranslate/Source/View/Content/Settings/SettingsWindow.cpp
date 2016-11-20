#include "View\Content\Settings\SettingsWindow.h"
#include "View\Controls\Inputs\HotKeyInputWindow.h"
#include "View\Controls\Buttons\HoverFlatButtonWindow.h"
#include "View\Content\Settings\HotkeySettingsWindow.h"

SettingsWindow::SettingsWindow(WindowContext* context, Window* parentWindow)
    : ContentWindow(context, parentWindow)
{
    stateMap[0] = true;
    stateMap[1] = true;
}

void SettingsWindow::Initialize()
{
    ContentWindow::Initialize();
}

Size SettingsWindow::RenderContent(Renderer* renderer)
{
    ContentWindow::RenderContent(renderer);
    DestroyChildWindows();

    int curY = paddingY;

    HotkeySettingsWindow* hotkeySettings = new HotkeySettingsWindow(context, this);
    curY = InitializeSettingsGroup(renderer, hotkeySettings, curY, 0) + lineHeight / 2;

    HotkeySettingsWindow* hotkeySettings2 = new HotkeySettingsWindow(context, this);
    curY = InitializeSettingsGroup(renderer, hotkeySettings2, curY, 1);

    curY += lineHeight / 2;

    HoverFlatButtonWindow* saveButton = new HoverFlatButtonWindow(context, this);
    saveButton->SetDimensions(Point(paddingX, curY), Size(50, 21));
    saveButton->SetText(L"Save");
    AddChildWindow(saveButton);

    renderer->IncreaseWidth(paddingX);
    renderer->IncreaseHeight(paddingY);

    return renderer->GetScaledSize();
}

int SettingsWindow::InitializeSettingsGroup(Renderer* renderer, SettingsGroupWindow* settingsGroup, int curY, int index)
{
    settingsGroup->SetDimensions(Point(paddingX, curY), 250);
    settingsGroup->SetState(stateMap[index]);
    settingsGroup->OnSettingsToggled.Subscribe([index, this]() -> void
    {
        stateMap[index] ^= true;
        this->Render(true);
    });

    AddChildWindow(settingsGroup);
    settingsGroup->Render();

    renderer->UpdateSize(Size(
        settingsGroup->GetSize().Width + settingsGroup->GetPosition().X,
        settingsGroup->GetSize().Height + settingsGroup->GetPosition().Y));

    return curY + settingsGroup->GetSize(true).Height;
}

SettingsWindow::~SettingsWindow()
{
}