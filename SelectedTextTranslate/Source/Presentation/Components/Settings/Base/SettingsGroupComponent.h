#pragma once
#include "Presentation\Framework\Component.h"
#include "Presentation\Components\Settings\Base\SettingsGroupView.h"

template<typename TSettings, typename TView>
class SettingsGroupComponent : public Component<TView>
{
private:
    void ProcessToggle();
    void ProcessChange();
    void UpdateState();

protected:
    ModelHolder<SettingsGroupViewModel<TSettings>*>* modelHolder;

public:
    SettingsGroupComponent(CommonContext* context, TView* view, ModelHolder<SettingsGroupViewModel<TSettings>*>* modelHolder);

    void Initialize() override;
    virtual bool IsValid() = 0;

    Subscribeable<> OnSettingsChanged;
};

template <typename TSettings, typename TView>
SettingsGroupComponent<TSettings, TView>::SettingsGroupComponent(CommonContext* context, TView* view, ModelHolder<SettingsGroupViewModel<TSettings>*>* modelHolder)
    : Component<TView>(context, view)
{
    this->modelHolder = modelHolder;
    this->view->OnSettingsChanged.Subscribe(bind(&SettingsGroupComponent::ProcessChange, this));
    this->view->OnSettingsToggled.Subscribe(bind(&SettingsGroupComponent::ProcessToggle, this));
}

template <typename TSettings, typename TView>
void SettingsGroupComponent<TSettings, TView>::Initialize()
{
    Component<TView>::Initialize();
    UpdateState();
}

template <typename TSettings, typename TView>
void SettingsGroupComponent<TSettings, TView>::ProcessToggle()
{
    SettingsGroupViewModel<TSettings>* settingsGroupViewModel = this->modelHolder->GetModel();
    SettingsGroupVisibilityState visibilityState = settingsGroupViewModel->GetVisibilityState() == SettingsGroupVisibilityState::Collapsed
        ? SettingsGroupVisibilityState::Expanded
        : SettingsGroupVisibilityState::Collapsed;
    settingsGroupViewModel->SetVisibilityState(visibilityState);
    this->Render(true);
}

template <typename TSettings, typename TView>
void SettingsGroupComponent<TSettings, TView>::ProcessChange()
{
    UpdateState();
    this->view->UpdateHeader();
    OnSettingsChanged.Notify();
}

template <typename TSettings, typename TView>
void SettingsGroupComponent<TSettings, TView>::UpdateState()
{
    SettingsGroupContentState contentState;
    if (!IsValid())
    {
        contentState = SettingsGroupContentState::Invalid;
    }
    else if (modelHolder->GetModel()->HasChanges())
    {
        contentState = SettingsGroupContentState::Modified;
    }
    else
    {
        contentState = SettingsGroupContentState::Default;
    }

    modelHolder->GetModel()->SetContentState(contentState);
}