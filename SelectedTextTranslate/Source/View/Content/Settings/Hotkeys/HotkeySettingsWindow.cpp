#include "View\Controls\Inputs\HotKeyInputWindow.h"
#include "View\Content\Settings\Hotkeys\HotkeySettingsWindow.h"
#include "View\Framework\Rendering\Dto\RenderResult.h"

HotkeySettingsWindow::HotkeySettingsWindow(WindowContext* context, Window* parentWindow)
    : SettingsGroupWindow(context, parentWindow)
{
    this->title = L"Hotkeys";
    this->OnSettingsChanged = Subscribeable<HotkeySettings>();
}

void HotkeySettingsWindow::RenderSettingsContent(RenderDescriptor renderDescriptor)
{
    RenderPosition renderPosition = renderDescriptor.GetRenderPosition().MoveY(paddingY);
    Renderer* renderer = renderDescriptor.GetRenderer();

    const int controlsMargin = 7;

    renderPosition = RenderHotkeyEditControl(
        RenderDescriptor(renderer, renderPosition),
        L"Translate Selected Text:",
        model.GetTranslateHotkey(),
        [this](DWORD hotkey) -> void
        {
            model.SetTranslateHotkey(hotkey);
        });

    renderPosition = RenderHotkeyEditControl(
        RenderDescriptor(renderer, renderPosition.MoveY(controlsMargin)),
        L"Play Selected Text:",
        model.GetPlayTextHotkey(),
        [this](DWORD hotkey) -> void
        {
            model.SetPlayTextHotkey(hotkey);
        });

    renderPosition = RenderHotkeyEditControl(
        RenderDescriptor(renderer, renderPosition.MoveY(controlsMargin)),
        L"Zoom In:",
        model.GetZoomInHotkey(),
        [this](DWORD hotkey) -> void
        {
            model.SetZoomInHotkey(hotkey);
        });

    RenderHotkeyEditControl(
        RenderDescriptor(renderer, renderPosition.MoveY(controlsMargin)),
        L"Zoom Out:",
        model.GetZoomOutHotkey(),
        [this](DWORD hotkey) -> void
        {
            model.SetZoomOutHotkey(hotkey);
        });

    renderer->IncreaseHeight(2 * paddingY);
}

RenderResult HotkeySettingsWindow::RenderHotkeyEditControl(RenderDescriptor renderDescriptor, wstring title, int hotkey, function<void(DWORD)> hotkeySetter)
{
    HFONT font = context->GetRenderingContext()->CreateCustomFont(FontSizes::Medium);
    int normalFontAscent = renderDescriptor.GetRenderer()->GetFontAscent(font);
    RenderPosition renderPosition = renderDescriptor.GetRenderPosition();

    TextRenderResult textRenderResult = renderDescriptor.GetRenderer()->PrintText(title.c_str(), font, Colors::Black, renderPosition.MoveY(normalFontAscent));

    renderPosition = renderPosition.SetY(textRenderResult.GetBottomY()).MoveY(1);

    HotKeyInputWindow* hotKeyInputWindow = new HotKeyInputWindow(context, this);
    hotKeyInputWindow->SetPosition(renderPosition.GetPosition());
    hotKeyInputWindow->SetDefaultHotkey(hotkey);
    hotKeyInputWindow->OnHotkeyChanged.Subscribe([hotkeySetter, this](DWORD newHotkey) -> void
    {
        hotkeySetter(newHotkey);
        this->OnSettingsChanged.Notify(model);
    });
    AddChildWindow(hotKeyInputWindow);

    renderDescriptor.GetRenderer()->UpdateRenderedContentSize(hotKeyInputWindow);

    return RenderResult(renderPosition.MoveY(hotKeyInputWindow->GetSize().Height));
}

HotkeySettingsWindow::~HotkeySettingsWindow()
{
}