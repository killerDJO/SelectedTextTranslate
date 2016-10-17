#include "ErrorHandling\Exceptions\Base\SelectedTextTranslateBaseException.h"
#include "Utilities\StringUtilities.h"

SelectedTextTranslateBaseException::SelectedTextTranslateBaseException(wstring message, const wchar_t* file, unsigned int line)
    : runtime_error(StringUtilities::GetUtf8String(message))
{
    this->FileName = wstring(file);
    this->LineNumber = line;
}

wstring SelectedTextTranslateBaseException::GetFullErrorMessage() const
{
    return StringUtilities::Format(L"%ls at %ls:%d", StringUtilities::GetUtf16String(what()).c_str(), FileName.c_str(), LineNumber);
}

wstring SelectedTextTranslateBaseException::GetDisplayErrorMessage() const
{
    return StringUtilities::GetUtf16String(what());
}