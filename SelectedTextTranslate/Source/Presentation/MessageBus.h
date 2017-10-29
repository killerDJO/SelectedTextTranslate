#pragma once
#include "Utilities\Subscribable\Subscribable.h"

class MessageBus
{
public:
    Subscribable<wstring, function<void()>> OnConfirmRequested;
    Subscribable<> OnSuspendHotkeys;
    Subscribable<> OnEnableHotkeys;

    Subscribable<> OnExit;
    Subscribable<> OnPlaySelectedText;
    Subscribable<> OnTranslateSelectedText;
    Subscribable<wstring, bool> OnTranslateText;

    Subscribable<> OnShowDictionary;
    Subscribable<> OnShowSettings;
    Subscribable<> OnShowTranslation;
};