#include "View\Content\Settings\Hotkeys\HotkeySettingsWindow.h"
#include "View\Framework\Rendering\Dto\RenderResult.h"

HotkeySettingsWindow::HotkeySettingsWindow(ViewContext* context, View* parentWindow)
    : SettingsGroupWindow(context, parentWindow)
{
    this->title = L"Hotkeys";
    this->font = context->GetRenderingProvider()->CreateCustomFont(FontSizes::Medium);
    this->controlsMargin = context->GetScaleProvider()->Scale(7);
}

void HotkeySettingsWindow::RenderSettingsContent(RenderDescriptor renderDescriptor)
{
    RenderPosition renderPosition = renderDescriptor.GetRenderPosition().MoveY(paddingY);
    Renderer* renderer = renderDescriptor.GetRenderer();

    hotkeyInputWindows.clear();

    renderPosition = RenderHotkeyEditControl(
        RenderDescriptor(renderer, renderPosition),
        L"Translate Selected Text:",
        currentSettings.GetHotkeySettings().GetTranslateHotkey(),
        [this](HotkeySettings* hotkeySettings, DWORD hotkey) -> void
    {
        hotkeySettings->SetTranslateHotkey(hotkey);
    });

    renderPosition = RenderHotkeyEditControl(
        RenderDescriptor(renderer, renderPosition.MoveY(controlsMargin)),
        L"Play Selected Text:",
        currentSettings.GetHotkeySettings().GetPlayTextHotkey(),
        [this](HotkeySettings* hotkeySettings, DWORD hotkey) -> void
    {
        hotkeySettings->SetPlayTextHotkey(hotkey);
    });

    renderPosition = RenderHotkeyEditControl(
        RenderDescriptor(renderer, renderPosition.MoveY(controlsMargin)),
        L"Zoom In:",
        currentSettings.GetHotkeySettings().GetZoomInHotkey(),
        [this](HotkeySettings* hotkeySettings, DWORD hotkey) -> void
    {
        hotkeySettings->SetZoomInHotkey(hotkey);
    });

    RenderHotkeyEditControl(
        RenderDescriptor(renderer, renderPosition.MoveY(controlsMargin)),
        L"Zoom Out:",
        currentSettings.GetHotkeySettings().GetZoomOutHotkey(),
        [this](HotkeySettings* hotkeySettings, DWORD hotkey) -> void
    {
        hotkeySettings->SetZoomOutHotkey(hotkey);
    });

    renderer->IncreaseHeight(2 * paddingY);
}

RenderResult HotkeySettingsWindow::RenderHotkeyEditControl(RenderDescriptor renderDescriptor, wstring title, int hotkey, function<void(HotkeySettings*, DWORD)> hotkeySetter)
{
    RenderPosition renderPosition = renderDescriptor.GetRenderPosition();

    TextRenderResult textRenderResult = renderDescriptor.GetRenderer()->PrintText(
        title.c_str(),
        font,
        Colors::Black,
        renderPosition.MoveY(font->GetAscent()));

    renderPosition = renderPosition
        .SetY(textRenderResult.GetBottomY())
        .MoveY(1);

    HotKeyInputWindow* hotKeyInputWindow = new HotKeyInputWindow(context, this);
    hotKeyInputWindow->SetPosition(renderPosition.GetPosition());
    hotKeyInputWindow->SetHotkey(hotkey);
    hotKeyInputWindow->OnHotkeyChanged.Subscribe([hotkeySetter, this](DWORD newHotkey)
    {
        HotkeySettings hotkeySettings = currentSettings.GetHotkeySettings();
        hotkeySetter(&hotkeySettings, newHotkey);
        currentSettings.SetHotkeySettings(hotkeySettings);

        ComputeContentState();
        OnSettingsChanged.Notify(currentSettings);
    });
    hotKeyInputWindow->InitializeAndRender();
    hotkeyInputWindows.push_back(hotKeyInputWindow);

    renderDescriptor.GetRenderer()->UpdateRenderedContentSize(hotKeyInputWindow);

    return RenderResult(renderPosition.MoveY(hotKeyInputWindow->GetSize().GetHeight()));
}

void HotkeySettingsWindow::ComputeContentState()
{
    bool isModified = !currentSettings.GetHotkeySettings().EqualTo(globalSettings.GetHotkeySettings());
    bool isValid = IsModelValid();

    SetHotkeyInputsValiationState();

    if (!isValid)
    {
        contentState = SettingsGroupContentState::Invalid;
    }
    else if (isModified)
    {
        contentState = SettingsGroupContentState::Modified;
    }
    else
    {
        contentState = SettingsGroupContentState::Default;
    }

    headerWindow->SetContentState(contentState);
    headerWindow->Render();
}

void HotkeySettingsWindow::SetHotkeyInputsValiationState()
{
    for (size_t i = 0; i < hotkeyInputWindows.size(); ++i)
    {
        hotkeyInputWindows[i]->MakeValid();
    }

    for (size_t i = 0; i < hotkeyInputWindows.size(); ++i)
    {
        for (size_t j = i + 1; j < hotkeyInputWindows.size(); ++j)
        {
            if (hotkeyInputWindows[i]->GetHotKey() == hotkeyInputWindows[j]->GetHotKey())
            {
                hotkeyInputWindows[i]->MakeInvalid();
                hotkeyInputWindows[j]->MakeInvalid();
            }
        }

        hotkeyInputWindows[i]->Render();
    }
}

bool HotkeySettingsWindow::IsModelValid() const
{
    bool isValid = true;

    vector<DWORD> hotkeys = vector<DWORD>();
    hotkeys.push_back(currentSettings.GetHotkeySettings().GetTranslateHotkey());
    hotkeys.push_back(currentSettings.GetHotkeySettings().GetPlayTextHotkey());
    hotkeys.push_back(currentSettings.GetHotkeySettings().GetZoomInHotkey());
    hotkeys.push_back(currentSettings.GetHotkeySettings().GetZoomOutHotkey());

    for (size_t i = 0; i < hotkeys.size(); ++i)
    {
        for (size_t j = i + 1; j < hotkeys.size(); ++j)
        {
            isValid = isValid && hotkeys[i] != hotkeys[j];
        }
    }

    return isValid;
}

HotkeySettingsWindow::~HotkeySettingsWindow()
{
    delete font;
}
