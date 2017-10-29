#pragma once
#include "Presentation\Controls\Buttons\HoverFlatButtonControl.h"
#include "Presentation\Controls\Buttons\HoverTextButtonControl.h"
#include "Presentation\Components\Settings\Base\SettingsGroupView.h"
#include "Presentation\Framework\ModelHolder.h"
#include "Presentation\Framework\Views\ChildComponentView.h"
#include "Presentation\Components\Settings\Base\SettingsGroupComponent.h"
#include "Presentation\Framework\Rendering\Dto\RenderResult.h"
#include "Presentation\Components\Settings\SettingsViewModel.h"

class SettingsView : public ChildComponentView<SettingsViewModel*>
{
private:
    HoverFlatButtonControl* saveButton;
    HoverTextButtonControl* cancelButton;
    HoverTextButtonControl* resetButton;

    vector<IComponent*> settingsGroups;

    RenderResult CreateSettingsGroups(RenderDescriptor renderDescriptor);
    RenderResult CreateHotkeySettingsGroup(RenderDescriptor renderDescriptor);

    RenderResult InitializeSettingsGroup(RenderDescriptor renderDescriptor, IComponent* settingsGroup);
    template<typename TSettings, typename TView>
    RenderResult AddSettingsGroup(RenderDescriptor renderDescriptor, SettingsGroupComponent<TSettings, TView>* settingsGroup);

    void CreateControls(RenderDescriptor renderDescriptor);
    RenderResult CreateSaveButtonControl(RenderDescriptor renderDescriptor);
    HoverTextButtonControl* CreateTextButtonControl(RenderDescriptor renderDescriptor, wstring text, Subscribable<>* clickCallback);

    void SetButtonsState() const;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    SettingsView(ViewContext* context, View* parentView, ModelHolder<SettingsViewModel*>* modelHolder, IComponent* component);

    Subscribable<> OnSaveSettings;
    Subscribable<> OnResetSettings;
    Subscribable<> OnCancelChanges;
};

template <typename TSettings, typename TView>
RenderResult SettingsView::AddSettingsGroup(RenderDescriptor renderDescriptor, SettingsGroupComponent<TSettings, TView>* settingsGroup)
{
    settingsGroup->OnSettingsChanged.Subscribe([this] { SetButtonsState(); });
    return InitializeSettingsGroup(renderDescriptor, settingsGroup);
}
