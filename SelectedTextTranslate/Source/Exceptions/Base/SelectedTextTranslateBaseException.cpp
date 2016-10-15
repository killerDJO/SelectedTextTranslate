#include "Exceptions\Base\SelectedTextTranslateBaseException.h"
#include "Helpers\StringUtilities.h"

SelectedTextTranslateBaseException::SelectedTextTranslateBaseException(wstring message, const wchar_t* file, unsigned int line)
    : runtime_error(StringUtilities::GetUtf8String(message))
{
    this->FileName = wstring(file);
    this->LineNumber = line;
}

wstring SelectedTextTranslateBaseException::GetErrorMessage() const
{
    return StringUtilities::Format(L"%ls at %ls:%d", StringUtilities::GetUtf16String(what()).c_str(), FileName.c_str(), LineNumber);
}
