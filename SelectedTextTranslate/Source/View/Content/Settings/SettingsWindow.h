#pragma once
#include "View\Framework\Windows\ContentWindow.h"
#include "View\Content\Settings\Base\SettingsGroupWindow.h"
#include "Services\Settings\Dto\Settings.h"
#include "View\Controls\Buttons\HoverFlatButtonWindow.h"
#include "View\Framework\ModelHolder.h"
#include "View\Content\Settings\Hotkeys\HotkeySettingsWindow.h"
#include "View\Content\Settings\Dto\SettingsState.h"
#include "View\Controls\Buttons\HoverTextButtonWindow.h"

class SettingsWindow : public ContentWindow, public ModelHolder<Settings>
{
private:
    Settings globalModel;
    SettingsState settingsState;

    HotkeySettingsWindow* hotkeySettingsWindow;

    HoverFlatButtonWindow* saveButton;
    HoverTextButtonWindow* cancelButton;
    HoverTextButtonWindow* resetButton;

    RenderResult CreateSettingsGroups(RenderDescriptor renderDescriptor);
    RenderResult CreateHotkeySettingsGroup(RenderDescriptor renderDescriptor);
    template<typename TModel>
    RenderResult InitializeSettingsGroup(RenderDescriptor renderDescriptor, SettingsGroupWindow* settingsGroup, SettingsGroupVisibilityState state, TModel model);

    void CreateControls(RenderDescriptor renderDescriptor);
    RenderResult CreateSaveButtonControl(RenderDescriptor renderDescriptor);
    HoverTextButtonWindow* CreateTextButtonControl(RenderDescriptor renderDescriptor, wstring text, function<void()> clickCallback);

    void UpdateSettings(Settings settings);
    void SetButtonsState() const;
    SettingsGroupVisibilityState ToggleSettingsGroupState(SettingsGroupVisibilityState settingsGroupState) const;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    SettingsWindow(WindowContext* context, Window* parentWindow);
    ~SettingsWindow() override;

    void SetModel(Settings model) override;

    Subscribeable<> OnSettingsStateChanged;
    Subscribeable<Settings> OnSaveSettings;
};
