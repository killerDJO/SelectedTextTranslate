#include "View\Framework\Providers\DialogsProvider.h"

DialogsProvider::DialogsProvider()
{
    this->OnConfirmRequested = Subscribeable<wstring, function<void()>>();
}

void DialogsProvider::ShowConfirmDialog(wstring title, function<void()> onConfirm)
{
    OnConfirmRequested.Notify(title, onConfirm);
}

DialogsProvider::~DialogsProvider()
{
}