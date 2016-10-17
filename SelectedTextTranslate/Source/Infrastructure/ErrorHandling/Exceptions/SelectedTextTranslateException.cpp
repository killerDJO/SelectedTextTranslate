#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateException.h"

SelectedTextTranslateException::SelectedTextTranslateException(wstring message, const wchar_t* file, unsigned int line)
    : SelectedTextTranslateBaseException(message, file, line)
{
}
