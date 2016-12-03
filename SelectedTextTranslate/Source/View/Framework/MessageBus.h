#pragma once
#include "Utilities\Subscribeable.h"

class MessageBus
{
public:
    MessageBus();
    ~MessageBus();

    void ShowConfirmDialog(wstring title, function<void()> onConfirm);
    void SuspendHotkeys();
    void EnableHotkeys();

    Subscribeable<wstring, function<void()>> OnConfirmRequested;
    Subscribeable<> OnSuspendHotkeys;
    Subscribeable<> OnEnableHotkeys;
};