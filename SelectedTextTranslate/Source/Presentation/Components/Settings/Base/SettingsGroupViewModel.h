#pragma once
#include "Presentation\Components\Settings\Base\Enums\SettingsGroupContentState.h"
#include "Presentation\Components\Settings\Base\Enums\SettingsGroupVisibilityState.h"
#include "Presentation\Framework\ModelHolder.h"

template<class TSettings>
class SettingsGroupViewModel : public ModelHolder<SettingsGroupViewModel<TSettings>*>
{
private:
    SettingsGroupContentState contentState;
    SettingsGroupVisibilityState visibilityState;
    wstring name;
    TSettings originalSettings;
    TSettings settings;

public:
    SettingsGroupViewModel(TSettings settings, wstring name);

    void CancelChanges();

    TSettings* GetSettings();

    void SetSettings(TSettings settings);

    SettingsGroupContentState GetContentState() const;
    void SetContentState(SettingsGroupContentState contentState);

    void SetVisibilityState(SettingsGroupVisibilityState visibilityState);
    SettingsGroupVisibilityState GetVisibilityState() const;

    wstring GetName() const;

    SettingsGroupViewModel<TSettings>* GetModel() override;

    bool HasChanges() const;
};

template <class TSettings>
SettingsGroupViewModel<TSettings>::SettingsGroupViewModel(TSettings settings, wstring name)
{
    this->settings = this->originalSettings = settings;
    this->name = name;
    contentState = SettingsGroupContentState::Default;
    visibilityState = SettingsGroupVisibilityState::Collapsed;
}

template <class TSettings>
void SettingsGroupViewModel<TSettings>::CancelChanges()
{
    settings = originalSettings;
}

template <class TSettings>
TSettings* SettingsGroupViewModel<TSettings>::GetSettings()
{
    return &settings;
}

template <class TSettings>
void SettingsGroupViewModel<TSettings>::SetSettings(TSettings settings)
{
    this->settings = this->originalSettings = settings;
}

template <class TSettings>
SettingsGroupContentState SettingsGroupViewModel<TSettings>::GetContentState() const
{
    return contentState;
}

template <class TSettings>
void SettingsGroupViewModel<TSettings>::SetContentState(SettingsGroupContentState contentState)
{
    this->contentState = contentState;
}

template <class TSettings>
void SettingsGroupViewModel<TSettings>::SetVisibilityState(SettingsGroupVisibilityState visibilityState)
{
    this->visibilityState = visibilityState;
}

template <class TSettings>
SettingsGroupVisibilityState SettingsGroupViewModel<TSettings>::GetVisibilityState() const
{
    return visibilityState;
}

template <class TSettings>
wstring SettingsGroupViewModel<TSettings>::GetName() const
{
    return name;
}

template <class TSettings>
SettingsGroupViewModel<TSettings>* SettingsGroupViewModel<TSettings>::GetModel()
{
    return this;
}

template <class TSettings>
bool SettingsGroupViewModel<TSettings>::HasChanges() const
{
    return !originalSettings.EqualTo(settings);
}