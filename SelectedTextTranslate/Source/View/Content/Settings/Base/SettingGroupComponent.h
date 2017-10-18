#pragma once
#include "View\Framework\View\Component.h"
#include "View\Content\Settings\Base\SettingsGroupView.h"

template<typename TSettings>
class SettingGroupComponent : public Component<SettingsGroupView<TSettings>>, public ModelHolder<SettingsGroupState<TSettings>*>
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
    SettingGroupComponent(ViewContext* context, View* parentView, ModelHolder<TSettings>* modelHolder);

    virtual bool IsValid() const = 0;
    virtual bool HasChanges() const = 0;

    TSettings GetCurrentSettings() const;
    void ResetCurrentSettings();

    SettingsGroupState<TSettings>* GetModel() override;

    Subscribeable<> OnSettingsChanged;
};

template <typename TSettings>
SettingGroupComponent<TSettings>::SettingGroupComponent(ViewContext* context, View* parentView, ModelHolder<TSettings>* modelHolder)
    : Component<SettingsGroupView<TSettings>>(context, new SettingsGroupView<TSettings>(context, parentView, this))
{
    this->modelHolder = modelHolder;
    this->view->OnSettingsChanged.Subscribe(bind(&SettingGroupComponent::ProcessChange, this));
    this->view->OnSettingsToggled.Subscribe(bind(&SettingGroupComponent::ProcessToggle, this));
}

template <typename TSettings>
void SettingGroupComponent<TSettings>::EnsureSettingsFetched()
{
    if (settingsGroupState.IsEmptyState())
    {
        settingsGroupState.SetSettings(modelHolder->GetModel());
        settingsGroupState.SetTitle(title);
    }
}

template <typename TSettings>
void SettingGroupComponent<TSettings>::ProcessToggle()
{
    SettingsGroupVisibilityState visibilityState = this->settingsGroupState.GetVisibilityState() == SettingsGroupVisibilityState::Collapsed
        ? SettingsGroupVisibilityState::Expanded
        : SettingsGroupVisibilityState::Collapsed;
    this->settingsGroupState.SetVisibilityState(visibilityState);
    this->view->Render(true);
}

template <typename TSettings>
void SettingGroupComponent<TSettings>::ProcessChange()
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

template <typename TSettings>
TSettings SettingGroupComponent<TSettings>::GetCurrentSettings() const
{
    this->EnsureSettingsFetched();
    return settingsGroupState.GetSettings();
}

template <typename TSettings>
void SettingGroupComponent<TSettings>::ResetCurrentSettings()
{
    settingsGroupState.SetSettings(modelHolder->GetModel());
}

template <typename TSettings>
SettingsGroupState<TSettings>* SettingGroupComponent<TSettings>::GetModel()
{
    this->EnsureSettingsFetched();
    return &settingsGroupState;
}