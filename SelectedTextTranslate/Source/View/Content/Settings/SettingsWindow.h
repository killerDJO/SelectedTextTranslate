#pragma once
#include "View\Content\Base\ContentWindow.h"
#include "View\Content\Settings\Base\SettingsGroupWindow.h"
#include "Services\Settings\Dto\Settings.h"
#include "View\Content\Settings\HotkeySettingsWindow.h"
#include "View\Controls\Buttons\HoverFlatButtonWindow.h"

class SettingsWindow : public ContentWindow, public ModelHolder<Settings>
{
private:
    HFONT fontSmallUnderscored;

    Settings globalModel;
    map<int, bool> settingsGroupsState;

    HotkeySettingsWindow* hotkeySettingsWindow;
    HoverFlatButtonWindow* saveButton;

    template<typename TModel>
    int InitializeSettingsGroup(Renderer* renderer, SettingsGroupWindow* settingsGroup, int curY, int index, TModel model);
    void CreateControls(Renderer* renderer, int curY);

    void SetSaveButtonState() const;

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
