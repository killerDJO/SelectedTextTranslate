#include "View\Content\Settings\HotkeySettingsWindow.h"
#include "View\Controls\Inputs\HotKeyInputWindow.h"

HotkeySettingsWindow::HotkeySettingsWindow(WindowContext* context, Window* parentWindow)
    : SettingsGroupWindow(context, parentWindow)
{
    this->title = L"Hotkeys";
    this->OnSettingsChanged = Subscribeable<HotkeySettings>();
}

void HotkeySettingsWindow::Initialize()
{
    SettingsGroupWindow::Initialize();
}

void HotkeySettingsWindow::RenderSettingsContent(Renderer* renderer, Point contentPosition)
{
    int curY = contentPosition.Y + paddingY;
    int curX = contentPosition.X;
    curY = RenderHotkeysEditControl(renderer, L"Translate Selected Text:", curX, curY, model.GetTranslateHotkey(), [this](DWORD hotkey) -> void { this->model.SetTranslateHotkey(hotkey); });
    curY = RenderHotkeysEditControl(renderer, L"Play Selected Text:", curX, curY, model.GetPlayTextHotkey(), [this](DWORD hotkey) -> void { this->model.SetPlayTextHotkey(hotkey); });
    curY = RenderHotkeysEditControl(renderer, L"Zoom In:", curX, curY, model.GetZoomInHotkey(), [this](DWORD hotkey) -> void { this->model.SetZoomInHotkey(hotkey); });
    RenderHotkeysEditControl(renderer, L"Zoom Out:", curX, curY, model.GetZoomOutHotkey(), [this](DWORD hotkey) -> void { this->model.SetZoomOutHotkey(hotkey); });

    renderer->IncreaseHeight(2 * paddingY);
}

int HotkeySettingsWindow::RenderHotkeysEditControl(Renderer* renderer, wstring title, int curX, int curY, int hotkey, function<void(DWORD)> hotkeySetter)
{
    int normalFontAscent = renderer->GetFontAscent(fontNormal);
    Point textBottomRight = renderer->PrintText(title.c_str(), fontNormal, Colors::Black, Point(curX, curY + normalFontAscent));

    curY = textBottomRight.Y + 1;

    HotKeyInputWindow* hotKeyInputWindow = new HotKeyInputWindow(context, this);
    hotKeyInputWindow->SetPosition(Point(curX, curY));
    hotKeyInputWindow->SetDefaultHotkey(hotkey);
    hotKeyInputWindow->OnHotkeyChanged.Subscribe([hotkeySetter, this](DWORD newHotkey) -> void
    {
        hotkeySetter(newHotkey);
        this->OnSettingsChanged.Notify(model);
    });
    AddChildWindow(hotKeyInputWindow);

    renderer->UpdateSize(Size(
        hotKeyInputWindow->GetSize().Width + hotKeyInputWindow->GetPosition().X,
        hotKeyInputWindow->GetSize().Height + hotKeyInputWindow->GetPosition().Y));

    curY += hotKeyInputWindow->GetSize(true).Height + 7;

    return curY;
}

HotkeySettingsWindow::~HotkeySettingsWindow()
{
}