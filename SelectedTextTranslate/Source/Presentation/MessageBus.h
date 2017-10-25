#pragma once
#include "Utilities\Subscribeable.h"

class MessageBus
{
public:
    Subscribeable<wstring, function<void()>> OnConfirmRequested;
    Subscribeable<> OnSuspendHotkeys;
    Subscribeable<> OnEnableHotkeys;

    Subscribeable<> OnExit;
    Subscribeable<> OnPlaySelectedText;
    Subscribeable<> OnTranslateSelectedText;
    Subscribeable<> OnShowDictionary;
    Subscribeable<> OnShowSettings;
};