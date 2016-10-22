#include "Infrastructure\ErrorHandling\StackProvider.h"
#include "Utilities\StringUtilities.h"
#include <dbghelp.h>

StackProvider::StackProvider()
{
    currentCallStack = vector<CallstackEntry>();
}

void StackProvider::OnCallstackEntry(CallstackEntryType eType, CallstackEntry &entry)
{
    currentCallStack.push_back(entry);
}

vector<wstring> StackProvider::GetCallStack()
{
    currentCallStack.clear();
    ShowCallstack();

    int stackOffset = 2;
    vector<wstring> callStackRecords = vector<wstring>();
    for (size_t i = stackOffset; i < currentCallStack.size(); ++i)
    {
        CallstackEntry callStackEntry = currentCallStack[i];

        wstring modlueInfo = StringUtilities::Format(L"[%hs], %hs;", callStackEntry.symTypeString, callStackEntry.moduleName);

        wstring callStackRecord;
        if (callStackEntry.symType != SymNone)
        {
            callStackRecord += StringUtilities::Format(L"\t\t%ls %hs", modlueInfo.c_str(), callStackEntry.name);

            if (strlen(callStackEntry.lineFileName) > 0)
            {
                callStackRecord += StringUtilities::Format(
                    L" at '%ls:%d'",
                    GetShortFileName(StringUtilities::GetUtf16StringFromChar(callStackEntry.lineFileName)).c_str(),
                    callStackEntry.lineNumber);
            }

            callStackRecord += L"\n";
        }
        else
        {
            callStackRecord += StringUtilities::Format(L"\t\t%ls\n", modlueInfo.c_str());
        }

        callStackRecords.push_back(callStackRecord);
    }

    return callStackRecords;
}

wstring StackProvider::GetShortFileName(wstring fileName) const
{
    vector<wstring> fileNameParts = StringUtilities::Split(fileName, L'\\');

    if (fileNameParts.size() == 0)
    {
        return wstring();
    }

    const wstring rootFolderName = L"source";
    wstring result = fileNameParts[fileNameParts.size() - 1];
    for (int i = fileNameParts.size() - 2; i >= 0; --i)
    {
        if (fileNameParts[i] == rootFolderName)
        {
            break;
        }

        result = fileNameParts[i] + L"\\" + result;
    }

    return result;
}

StackProvider::~StackProvider()
{
}