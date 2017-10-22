#pragma once
#include "View\Controls\Buttons\HoverFlatButtonWindow.h"
#include "View\Controls\Buttons\HoverTextButtonWindow.h"
#include "View\Content\Settings\Base\SettingsGroupView.h"
#include "View\Framework\ModelHolder.h"
#include "View\Framework\View\Views\ComponentView.h"
#include "View\Content\Settings\Base\SettingGroupComponent.h"
#include "View\Framework\Rendering\Dto\RenderResult.h"
#include "View\Content\Settings\SettingsViewModel.h"

class SettingsView : public ComponentView<SettingsViewModel*>
{
private:
    HoverFlatButtonWindow* saveButton;
    HoverTextButtonWindow* cancelButton;
    HoverTextButtonWindow* resetButton;

    vector<ChildView*> settingsGroups;

    RenderResult CreateSettingsGroups(RenderDescriptor renderDescriptor);
    RenderResult CreateHotkeySettingsGroup(RenderDescriptor renderDescriptor);

    template<typename TSettings, typename TView>
    RenderResult InitializeSettingsGroup(RenderDescriptor renderDescriptor, SettingGroupComponent<TSettings, TView>* settingsGroup);

    void CreateControls(RenderDescriptor renderDescriptor);
    RenderResult CreateSaveButtonControl(RenderDescriptor renderDescriptor);
    HoverTextButtonWindow* CreateTextButtonControl(RenderDescriptor renderDescriptor, wstring text, Subscribeable<>* clickCallback);

    void SetButtonsState() const;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    SettingsView(ViewContext* context, View* parentView, ModelHolder<SettingsViewModel*>* modelHolder);

    Subscribeable<> OnSaveSettings;
    Subscribeable<> OnResetSettings;
    Subscribeable<> OnCancelChanges;
};

template <typename TSettings, typename TView>
RenderResult SettingsView::InitializeSettingsGroup(RenderDescriptor renderDescriptor, SettingGroupComponent<TSettings, TView>* settingsGroup)
{
    settingsGroups.push_back(settingsGroup->GetView());
    settingsGroup->SetDescriptor(WindowDescriptor::CreateWindowDescriptor(
        renderDescriptor.GetRenderPosition().GetPosition(),
        Size(scaleProvider->Scale(257), 0),
        OverflowModes::Fixed,
        OverflowModes::Stretch));

    settingsGroup->OnSettingsChanged.Subscribe([this]()
    {
        SetButtonsState();
    });

    settingsGroup->InitializeAndRender();

    renderDescriptor.GetRenderer()->UpdateRenderedContentSize(settingsGroup->GetView());

    return RenderResult(settingsGroup->GetBoundingRect());
}
