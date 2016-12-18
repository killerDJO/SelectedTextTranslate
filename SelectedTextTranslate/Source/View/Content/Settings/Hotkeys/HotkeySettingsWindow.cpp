#include "View\Content\Settings\Hotkeys\HotkeySettingsWindow.h"
#include "View\Framework\Rendering\Dto\RenderResult.h"

HotkeySettingsWindow::HotkeySettingsWindow(WindowContext* context, Window* parentWindow)
    : SettingsGroupWindow(context, parentWindow)
{
    this->title = L"Hotkeys";
    this->OnSettingsChanged = Subscribeable<HotkeySettings>();
    this->font = context->GetRenderingContext()->CreateCustomFont(FontSizes::Medium);
}

void HotkeySettingsWindow::SetModel(HotkeySettings model)
{
    ModelHolder<HotkeySettings>::SetModel(model);
    globalModel = model;
}

void HotkeySettingsWindow::RenderSettingsContent(RenderDescriptor renderDescriptor)
{
    RenderPosition renderPosition = renderDescriptor.GetRenderPosition().MoveY(paddingY);
    Renderer* renderer = renderDescriptor.GetRenderer();

    const int controlsMargin = 7;

    hotkeyInputWindows.clear();

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
        hotkeySetter(newHotkey);
        ComputeContentState();
        OnSettingsChanged.Notify(model);
    });
    AddChildWindow(hotKeyInputWindow);
    hotkeyInputWindows.push_back(hotKeyInputWindow);

    renderDescriptor.GetRenderer()->UpdateRenderedContentSize(hotKeyInputWindow);

    return RenderResult(renderPosition.MoveY(hotKeyInputWindow->GetSize().GetHeight()));
}

void HotkeySettingsWindow::ComputeContentState()
{
    bool isModified = !model.EqualTo(globalModel);
    bool isValid = IsModelValid();

    ValidateHotkeyInputs();

    if(!isValid)
    {
        contentState = SettingsGroupContentState::Invalid;
    }
    else if(isModified)
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

void HotkeySettingsWindow::ValidateHotkeyInputs()
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
    hotkeys.push_back(model.GetTranslateHotkey());
    hotkeys.push_back(model.GetPlayTextHotkey());
    hotkeys.push_back(model.GetZoomInHotkey());
    hotkeys.push_back(model.GetZoomOutHotkey());

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
