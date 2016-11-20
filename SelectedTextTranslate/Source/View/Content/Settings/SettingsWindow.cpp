#include "View\Content\Settings\SettingsWindow.h"
#include "View\Controls\Inputs\HotKeyInputWindow.h"
#include "View\Controls\Buttons\HoverFlatButtonWindow.h"
#include "View\Content\Settings\HotkeySettingsWindow.h"

SettingsWindow::SettingsWindow(WindowContext* context, Window* parentWindow)
    : ContentWindow(context, parentWindow)
{
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
    hotkeySettings->SetDimensions(Point(paddingX, curY), 250);
    hotkeySettings->SetState(true);
    hotkeySettings->SetTitle(L"Hotkeys settings");
    AddChildWindow(hotkeySettings);
    hotkeySettings->Render();

    curY += context->GetScaleProvider()->Downscale(hotkeySettings->GetSize().Height) - 1;

    HotkeySettingsWindow* hotkeySettings2 = new HotkeySettingsWindow(context, this);
    hotkeySettings2->SetDimensions(Point(paddingX, curY), 250);
    hotkeySettings2->SetState(false);
    hotkeySettings2->SetTitle(L"Hotkeys settings");
    AddChildWindow(hotkeySettings2);
    hotkeySettings2->Render();

    curY += context->GetScaleProvider()->Downscale(hotkeySettings2->GetSize().Height);
    curY += lineHeight / 2;

    HoverFlatButtonWindow* saveButton = new HoverFlatButtonWindow(context, this);
    saveButton->SetDimensions(Point(paddingX, curY), Size(50, 21));
    saveButton->SetText(L"Save");
    AddChildWindow(saveButton);

    renderer->IncreaseWidth(paddingX);
    renderer->IncreaseHeight(paddingY);

    return renderer->GetScaledSize();
}

SettingsWindow::~SettingsWindow()
{
}