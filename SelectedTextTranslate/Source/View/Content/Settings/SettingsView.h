#pragma once
#include "Services\Settings\Dto\Settings.h"
#include "View\Controls\Buttons\HoverFlatButtonWindow.h"
#include "View\Controls\Buttons\HoverTextButtonWindow.h"
#include "View\Content\Settings\Base\SettingsGroupView.h"
#include "View\Content\Settings\Hotkeys\HotkeySettingsView.h"
#include "View\Framework\ModelHolder.h"
#include "View\Framework\View\Views\ComponentView.h"
#include "View\Content\Settings\Base\SettingGroupComponent.h"
#include "View\Content\Settings\Hotkeys\HotkeySettingsComponent.h"
#include "View\Framework\Rendering\Dto\RenderResult.h"

class SettingsView : public ComponentView<Settings>, public ModelHolder<HotkeySettings>
{
private:
    HotkeySettingsComponent* hotkeySettingsComponent;

    HoverFlatButtonWindow* saveButton;
    HoverTextButtonWindow* cancelButton;
    HoverTextButtonWindow* resetButton;

    RenderResult CreateSettingsGroups(RenderDescriptor renderDescriptor);
    RenderResult CreateHotkeySettingsGroup(RenderDescriptor renderDescriptor);

    template<typename TSettings>
    RenderResult InitializeSettingsGroup(RenderDescriptor renderDescriptor, SettingGroupComponent<TSettings>* settingsGroup);

    void CreateControls(RenderDescriptor renderDescriptor);
    RenderResult CreateSaveButtonControl(RenderDescriptor renderDescriptor);
    HoverTextButtonWindow* CreateTextButtonControl(RenderDescriptor renderDescriptor, wstring text, function<void()> clickCallback);

    void CancelSettingsChanges() const;
    void SetButtonsState() const;

    Settings GetCurrentSettings() const;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    SettingsView(ViewContext* context, View* parentView, ModelHolder<Settings>* modelHolder);

    HotkeySettings GetModel() override;

    Subscribeable<Settings> OnSaveSettings;
    Subscribeable<> OnResetSettings;
};

template <typename TSettings>
RenderResult SettingsView::InitializeSettingsGroup(RenderDescriptor renderDescriptor, SettingGroupComponent<TSettings>* settingsGroup)
{
    settingsGroup->SetDescriptor(WindowDescriptor::CreateWindowDescriptor(
        nativeStateDescriptor.GetPosition(),
        Size(scaleProvider->Scale(257), 0),
        OverflowModes::Fixed,
        OverflowModes::Stretch));

    settingsGroup->OnSettingsChanged.Subscribe([this]()
    {
        SetButtonsState();
    });

    settingsGroup->InitializeAndRender();

    renderDescriptor.GetRenderer()->UpdateRenderedContentSize(settingsGroup);

    return RenderResult(settingsGroup->GetBoundingRect());
}
