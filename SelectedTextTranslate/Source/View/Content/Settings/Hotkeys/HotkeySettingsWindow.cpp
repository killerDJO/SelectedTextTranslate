#include "View\Controls\Inputs\HotKeyInputWindow.h"
#include "View\Content\Settings\Hotkeys\HotkeySettingsWindow.h"

HotkeySettingsWindow::HotkeySettingsWindow(WindowContext* context, Window* parentWindow)
    : SettingsGroupWindow(context, parentWindow)
{
    this->title = L"Hotkeys";
    this->OnSettingsChanged = Subscribeable<HotkeySettings>();
}

void HotkeySettingsWindow::RenderSettingsContent(Renderer* renderer, Point contentPosition)
{
    int curY = contentPosition.Y + paddingY;
    int curX = contentPosition.X;
    curY = RenderHotkeyEditControl(renderer, L"Translate Selected Text:", curX, curY, model.GetTranslateHotkey(), [this](DWORD hotkey) -> void { this->model.SetTranslateHotkey(hotkey); });
    curY = RenderHotkeyEditControl(renderer, L"Play Selected Text:", curX, curY, model.GetPlayTextHotkey(), [this](DWORD hotkey) -> void { this->model.SetPlayTextHotkey(hotkey); });
    curY = RenderHotkeyEditControl(renderer, L"Zoom In:", curX, curY, model.GetZoomInHotkey(), [this](DWORD hotkey) -> void { this->model.SetZoomInHotkey(hotkey); });
    RenderHotkeyEditControl(renderer, L"Zoom Out:", curX, curY, model.GetZoomOutHotkey(), [this](DWORD hotkey) -> void { this->model.SetZoomOutHotkey(hotkey); });

    renderer->IncreaseHeight(2 * paddingY + 2);
}

int HotkeySettingsWindow::RenderHotkeyEditControl(Renderer* renderer, wstring title, int curX, int curY, int hotkey, function<void(DWORD)> hotkeySetter)
{
    HFONT font = context->GetRenderingContext()->CreateCustomFont(FontSizes::Medium);
    int normalFontAscent = renderer->GetFontAscent(font);
    Point textBottomRight = renderer->PrintText(title.c_str(), font, Colors::Black, Point(curX, curY + normalFontAscent));

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

    renderer->UpdateRenderedContentSize(hotKeyInputWindow);

    curY += hotKeyInputWindow->GetSize(true).Height + 7;

    return curY;
}

HotkeySettingsWindow::~HotkeySettingsWindow()
{
}