#pragma once
#include "View\Content\Base\ContentWindow.h"
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
    HFONT fontSmallUnderscored;

    Settings globalModel;
    SettingsState settingsState;

    HotkeySettingsWindow* hotkeySettingsWindow;

    HoverFlatButtonWindow* saveButton;
    HoverTextButtonWindow* cancelButton;
    HoverTextButtonWindow* resetButton;

    int CreateSettingsGroups(Renderer* renderer, int curY);
    int CreateHotkeySettingsGroup(Renderer* renderer, int curY);
    template<typename TModel>
    int InitializeSettingsGroup(Renderer* renderer, SettingsGroupWindow* settingsGroup, int curY, SettingsGroupState state, TModel model);

    void CreateControls(Renderer* renderer, int curY, int curX);
    Point CreateSaveButtonControl(Renderer* renderer, int curY, int curX);
    HoverTextButtonWindow* CreateTextButtonControl(Renderer* renderer, Point position, wstring text, function<void()> clickCallback);

    void UpdateSettings(Settings settings);
    void SetButtonsState() const;
    void SetSettingsState() const;
    SettingsGroupState ToggleSettingsGroupState(SettingsGroupState settingsGroupState) const;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    SettingsWindow(WindowContext* context, Window* parentWindow);
    ~SettingsWindow() override;

    void Initialize() override;

    void SetModel(Settings model) override;

    Subscribeable<> OnSettingsStateChanged;
    Subscribeable<Settings> OnSaveSettings;
};
