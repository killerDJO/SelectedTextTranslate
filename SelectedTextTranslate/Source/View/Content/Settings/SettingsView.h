#pragma once
#include "View\Controls\Buttons\HoverFlatButtonWindow.h"
#include "View\Controls\Buttons\HoverTextButtonWindow.h"
#include "View\Content\Settings\Base\SettingsGroupView.h"
#include "View\Framework\ModelHolder.h"
#include "View\Framework\View\Views\ComponentView.h"
#include "View\Content\Settings\Base\SettingsGroupComponent.h"
#include "View\Framework\Rendering\Dto\RenderResult.h"
#include "View\Content\Settings\SettingsViewModel.h"

class SettingsView : public ComponentView<SettingsViewModel*>
{
private:
    HoverFlatButtonWindow* saveButton;
    HoverTextButtonWindow* cancelButton;
    HoverTextButtonWindow* resetButton;

    vector<IComponent*> settingsGroups;

    RenderResult CreateSettingsGroups(RenderDescriptor renderDescriptor);
    RenderResult CreateHotkeySettingsGroup(RenderDescriptor renderDescriptor);

    RenderResult InitializeSettingsGroup(RenderDescriptor renderDescriptor, IComponent* settingsGroup);
    template<typename TSettings, typename TView>
    RenderResult AddSettingsGroup(RenderDescriptor renderDescriptor, SettingsGroupComponent<TSettings, TView>* settingsGroup);

    void CreateControls(RenderDescriptor renderDescriptor);
    RenderResult CreateSaveButtonControl(RenderDescriptor renderDescriptor);
    HoverTextButtonWindow* CreateTextButtonControl(RenderDescriptor renderDescriptor, wstring text, Subscribeable<>* clickCallback);

    void SetButtonsState() const;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    SettingsView(CommonContext* context, View* parentView, ModelHolder<SettingsViewModel*>* modelHolder);

    Subscribeable<> OnSaveSettings;
    Subscribeable<> OnResetSettings;
    Subscribeable<> OnCancelChanges;
};

template <typename TSettings, typename TView>
RenderResult SettingsView::AddSettingsGroup(RenderDescriptor renderDescriptor, SettingsGroupComponent<TSettings, TView>* settingsGroup)
{
    settingsGroup->OnSettingsChanged.Subscribe([this] { SetButtonsState(); });
    return InitializeSettingsGroup(renderDescriptor, settingsGroup);
}
