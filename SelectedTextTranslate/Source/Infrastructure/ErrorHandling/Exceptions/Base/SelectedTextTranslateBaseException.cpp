#include "Infrastructure\ErrorHandling\Exceptions\Base\SelectedTextTranslateBaseException.h"
#include "Infrastructure\ErrorHandling\StackProvider.h"
#include "Utilities\StringUtilities.h"

SelectedTextTranslateBaseException::SelectedTextTranslateBaseException(wstring message)
    : runtime_error(StringUtilities::GetUtf8String(message))
{
    StackProvider stackProvider;
    vector<wstring> callStackRecords = stackProvider.GetCallStack();
    for(size_t i = 2; i < callStackRecords.size(); ++i)
    {
        callStack += callStackRecords[i];
    }
}

wstring SelectedTextTranslateBaseException::GetFullErrorMessage() const
{
    return StringUtilities::Format(L"\tException message: '%ls'.\n\tCall stack:\n%ls", GetDisplayErrorMessage().c_str(), callStack.c_str());
}

wstring SelectedTextTranslateBaseException::GetDisplayErrorMessage() const
{
    return StringUtilities::GetUtf16String(what());
}

SelectedTextTranslateBaseException::~SelectedTextTranslateBaseException()
{
}