#include "View\Content\Settings\SettingsWindow.h"
#include "View\Controls\Inputs\HotKeyInputWindow.h"
#include "View\Controls\Buttons\HoverFlatButtonWindow.h"
#include "View\Content\Settings\HotkeySettingsWindow.h"

SettingsWindow::SettingsWindow(WindowContext* context, WindowDescriptor descriptor, wstring name, Window* parentWindow)
    : ContentWindow(context, descriptor, name, parentWindow)
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

    int normalFontHeight = renderer->GetFontHeight(fontNormal);
    HotkeySettingsWindow* hotkeySettings = new HotkeySettingsWindow(
        context,
        Point(paddingX, curY),
        250,
        L"HotkeySettings",
        this,
        true,
        normalFontHeight);
    AddChildWindow(hotkeySettings);
    hotkeySettings->Render();

    curY += context->GetScaleProvider()->Downscale(hotkeySettings->GetSize().Height) - 1;

    HotkeySettingsWindow* hotkeySettings2 = new HotkeySettingsWindow(
        context,
        Point(paddingX, curY),
        250,
        L"HotkeySettings",
        this,
        false,
        normalFontHeight);
    AddChildWindow(hotkeySettings2);
    hotkeySettings2->Render();

    curY += context->GetScaleProvider()->Downscale(hotkeySettings2->GetSize().Height);
    curY += lineHeight / 2;

    HoverFlatButtonWindow* saveButton = new HoverFlatButtonWindow(
        context,
        WindowDescriptor::CreateFixedWindowDescriptor(
            Point(paddingX, curY),
            Size(50, 21)),
        L"SaveButton",
        this,
        fontNormal,
        L"Save");
    AddChildWindow(saveButton);

    renderer->IncreaseWidth(paddingX);
    renderer->IncreaseHeight(paddingY);

    return renderer->GetScaledSize();
}

SettingsWindow::~SettingsWindow()
{
}