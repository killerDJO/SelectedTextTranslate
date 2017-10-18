#pragma once
#include "View\Content\Settings\Base\Enums\SettingsGroupContentState.h"
#include "View\Content\Settings\Base\Enums\SettingsGroupVisibilityState.h"

template<typename TSettings>
class SettingsGroupState
{
private:
    SettingsGroupContentState contentState;
    SettingsGroupVisibilityState visibilityState;
    TSettings settings;
    wstring title;

    bool isEmptyState;

public:
    SettingsGroupState();
    SettingsGroupState(TSettings settings);

    void SetSettings(TSettings settings);
    TSettings GetSettings() const;

    SettingsGroupContentState GetContentState() const;
    void SetContentState(SettingsGroupContentState contentState);

    void SetVisibilityState(SettingsGroupVisibilityState visibilityState);
    SettingsGroupVisibilityState GetVisibilityState() const;

    void SetTitle(wstring title);
    wstring GetTitle() const;

    bool IsEmptyState() const;
};

template <typename TSettings>
SettingsGroupState<TSettings>::SettingsGroupState()
{
    isEmptyState = true;
    contentState = SettingsGroupContentState::Default;
    visibilityState = SettingsGroupVisibilityState::Collapsed;
}

template <typename TSettings>
SettingsGroupState<TSettings>::SettingsGroupState(TSettings settings)
{
    this->settings = settings;
    contentState = SettingsGroupContentState::Default;
    visibilityState = SettingsGroupVisibilityState::Collapsed;
    isEmptyState = false;
}

template <typename TSettings>
void SettingsGroupState<TSettings>::SetSettings(TSettings settings)
{
    isEmptyState = false;
    this->settings = settings;
}

template <typename TSettings>
TSettings SettingsGroupState<TSettings>::GetSettings() const
{
    return settings;
}

template <typename TSettings>
SettingsGroupContentState SettingsGroupState<TSettings>::GetContentState() const
{
    return contentState;
}

template <typename TSettings>
void SettingsGroupState<TSettings>::SetContentState(SettingsGroupContentState contentState)
{
    this->contentState = contentState;
}

template <typename TSettings>
void SettingsGroupState<TSettings>::SetVisibilityState(SettingsGroupVisibilityState visibilityState)
{
    this->visibilityState = visibilityState;
}

template <typename TSettings>
SettingsGroupVisibilityState SettingsGroupState<TSettings>::GetVisibilityState() const
{
    return visibilityState;
}

template <typename TSettings>
void SettingsGroupState<TSettings>::SetTitle(wstring title)
{
    this->title = title;
}

template <typename TSettings>
wstring SettingsGroupState<TSettings>::GetTitle() const
{
    return title;
}

template <typename TSettings>
bool SettingsGroupState<TSettings>::IsEmptyState() const
{
    return isEmptyState;
}
