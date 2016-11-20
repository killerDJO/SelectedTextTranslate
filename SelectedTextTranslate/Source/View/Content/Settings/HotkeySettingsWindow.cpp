#include "View\Content\Settings\HotkeySettingsWindow.h"
#include "View\Controls\Inputs\HotKeyInputWindow.h"

HotkeySettingsWindow::HotkeySettingsWindow(WindowContext* context, Window* parentWindow)
    : SettingGroupWindow(context, parentWindow)
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
    Point textBottomRight = renderer->PrintText(title.c_str(), fontNormal, Colors::Black, Point(curX, curY + normalFontAscent));

    curY = textBottomRight.Y + 1;

    HotKeyInputWindow* hotKeyInputWindow = new HotKeyInputWindow(context, this);
    hotKeyInputWindow->SetPosition(Point(curX, curY));
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