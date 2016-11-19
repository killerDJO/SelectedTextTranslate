#include "View\Content\Settings\HotkeySettingsWindow.h"
#include "View\Controls\Inputs\HotKeyInputWindow.h"

HotkeySettingsWindow::HotkeySettingsWindow(WindowContext* context, Point position, int width, wstring name, Window* parentWindow, bool isCollapsed, int titleLineHeight)
    : SettingGroupWindow(context, position, width, name, parentWindow, isCollapsed, L"Hotkeys Settings", titleLineHeight)
{
}

void HotkeySettingsWindow::Initialize()
{
    SettingGroupWindow::Initialize();
}

void HotkeySettingsWindow::RenderSettingsContent(Renderer* renderer, Point contentPosition)
{
    int curY = contentPosition.Y;
    int curX = contentPosition.X;
    curY = RenderHotkeysEditControl(renderer, L"Translate selected text hotkey:", curX, curY);
    RenderHotkeysEditControl(renderer, L"Play selected text hotkey:", curX, curY);
    renderer->IncreaseWidth(paddingY);
}

int HotkeySettingsWindow::RenderHotkeysEditControl(Renderer* renderer, wstring title, int curX, int curY)
{
    int normalFontAscent = renderer->GetFontAscent(fontNormal);
    renderer->PrintText(title.c_str(), fontNormal, Colors::Black, Point(curX, curY + normalFontAscent));

    curY += paddingY + 3;

    int hotkeyControlFontHeight = renderer->GetFontHeight(fontNormal);
    HotKeyInputWindow* hotKeyInputWindow = new HotKeyInputWindow(
        context,
        Point(curX, curY),
        L"HotKeyInput",
        this,
        0,
        fontNormal,
        hotkeyControlFontHeight);

    AddChildWindow(hotKeyInputWindow);

    renderer->UpdateSize(Size(
        hotKeyInputWindow->GetSize().Width + hotKeyInputWindow->GetPosition().X,
        hotKeyInputWindow->GetSize().Height + hotKeyInputWindow->GetPosition().Y));

    curY += context->GetScaleProvider()->Downscale(hotKeyInputWindow->GetSize().Height) + 7;

    return curY;
}

HotkeySettingsWindow::~HotkeySettingsWindow()
{
}