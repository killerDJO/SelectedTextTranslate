#pragma once
#include "Utilities\Subscribeable.h"

class DialogsProvider
{
public:
    DialogsProvider();
    ~DialogsProvider();

    void ShowConfirmDialog(wstring title, function<void()> onConfirm);

    Subscribeable<wstring, function<void()>> OnConfirmRequested;
};
