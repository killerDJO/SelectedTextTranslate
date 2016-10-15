#include "Exceptions\SelectedTextTranslateFatalException.h"

SelectedTextTranslateFatalException::SelectedTextTranslateFatalException(wstring message, const wchar_t* file, unsigned int line)
    : SelectedTextTranslateBaseException(message, file, line)
{
}
