#pragma once
#include "StackWalker.h"

class StackProvider : StackWalker
{
private:
    vector<CallstackEntry> currentCallStack;

    wstring GetShortFileName(wstring fileName) const;

protected:
    void OnCallstackEntry(CallstackEntryType eType, CallstackEntry &entry) override;

public:
    StackProvider();

    vector<wstring> GetCallStack();
};