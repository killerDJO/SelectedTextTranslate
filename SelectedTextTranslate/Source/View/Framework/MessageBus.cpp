#include "View\Framework\MessageBus.h"

MessageBus::MessageBus()
{
    this->OnConfirmRequested = Subscribeable<wstring, function<void()>>();
    this->OnSuspendHotkeys = Subscribeable<>();
    this->OnEnableHotkeys = Subscribeable<>();
}

void MessageBus::ShowConfirmDialog(wstring title, function<void()> onConfirm)
{
    OnConfirmRequested.Notify(title, onConfirm);
}

void MessageBus::SuspendHotkeys()
{
    OnSuspendHotkeys.Notify();
}

void MessageBus::EnableHotkeys()
{
    OnEnableHotkeys.Notify();
}

MessageBus::~MessageBus()
{
}
