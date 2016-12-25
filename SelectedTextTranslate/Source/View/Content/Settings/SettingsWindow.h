#pragma once
#include "Services\Settings\Dto\Settings.h"
#include "View\Framework\Windows\ContentWindow.h"
#include "View\Framework\ModelHolder.h"
#include "View\Controls\Buttons\HoverFlatButtonWindow.h"
#include "View\Controls\Buttons\HoverTextButtonWindow.h"
#include "View\Content\Settings\Base\Dto\SettingsState.h"
#include "View\Content\Settings\Base\SettingsGroupWindow.h"
#include "View\Content\Settings\Hotkeys\HotkeySettingsWindow.h"

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
    RenderResult InitializeSettingsGroup(RenderDescriptor renderDescriptor, SettingsGroupWindow* settingsGroup, SettingsGroupVisibilityState state);

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

    void SetModel(Settings model) override;

    Subscribeable<Settings> OnSaveSettings;
};
