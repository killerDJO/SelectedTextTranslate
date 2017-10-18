#pragma once
#include "View\Framework\View\Component.h"
#include "View\Content\Settings\Base\SettingsGroupView.h"

template<typename TSettings, typename TView>
class SettingGroupComponent : public Component<TView>, public ModelHolder<SettingsGroupState<TSettings>*>
{
private:
    void EnsureSettingsFetched();
    void ProcessToggle();
    void ProcessChange();

protected:
    wstring title;
    ModelHolder<TSettings>* modelHolder;
    SettingsGroupState<TSettings> settingsGroupState;

public:
    SettingGroupComponent(ViewContext* context, TView* view, ModelHolder<TSettings>* modelHolder);

    virtual bool IsValid() const = 0;
    virtual bool HasChanges() const = 0;

    TSettings GetCurrentSettings();
    void ResetCurrentSettings();

    SettingsGroupState<TSettings>* GetModel() override;

    Subscribeable<> OnSettingsChanged;
};

template<typename TSettings, typename TView>
SettingGroupComponent<TSettings, TView>::SettingGroupComponent(ViewContext* context, TView* view, ModelHolder<TSettings>* modelHolder)
    : Component<TView>(context, view)
{
    this->modelHolder = modelHolder;
    this->view->OnSettingsChanged.Subscribe(bind(&SettingGroupComponent::ProcessChange, this));
    this->view->OnSettingsToggled.Subscribe(bind(&SettingGroupComponent::ProcessToggle, this));
}

template <typename TSettings, typename TView>
void SettingGroupComponent<TSettings, TView>::EnsureSettingsFetched()
{
    if (settingsGroupState.IsEmptyState())
    {
        settingsGroupState.SetSettings(modelHolder->GetModel());
        settingsGroupState.SetTitle(title);
    }
}

template <typename TSettings, typename TView>
void SettingGroupComponent<TSettings, TView>::ProcessToggle()
{
    SettingsGroupVisibilityState visibilityState = this->settingsGroupState.GetVisibilityState() == SettingsGroupVisibilityState::Collapsed
        ? SettingsGroupVisibilityState::Expanded
        : SettingsGroupVisibilityState::Collapsed;
    this->settingsGroupState.SetVisibilityState(visibilityState);
    this->view->Render(true);
}

template <typename TSettings, typename TView>
void SettingGroupComponent<TSettings, TView>::ProcessChange()
{
    SettingsGroupContentState contentState;
    if (!IsValid())
    {
        contentState = SettingsGroupContentState::Invalid;
    }
    else if (HasChanges())
    {
        contentState = SettingsGroupContentState::Modified;
    }
    else
    {
        contentState = SettingsGroupContentState::Default;
    }

    this->settingsGroupState.SetContentState(contentState);
    this->view->Render(true);
    this->OnSettingsChanged.Notify();
}

template <typename TSettings, typename TView>
TSettings SettingGroupComponent<TSettings, TView>::GetCurrentSettings()
{
    this->EnsureSettingsFetched();
    return settingsGroupState.GetSettings();
}

template <typename TSettings, typename TView>
void SettingGroupComponent<TSettings, TView>::ResetCurrentSettings()
{
    settingsGroupState.SetSettings(modelHolder->GetModel());
}

template <typename TSettings, typename TView>
SettingsGroupState<TSettings>* SettingGroupComponent<TSettings, TView>::GetModel()
{
    this->EnsureSettingsFetched();
    return &settingsGroupState;
}