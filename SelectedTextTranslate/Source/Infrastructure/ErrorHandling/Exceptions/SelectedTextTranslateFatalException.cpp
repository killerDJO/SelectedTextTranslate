#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"

SelectedTextTranslateFatalException::SelectedTextTranslateFatalException(wstring message)
    : SelectedTextTranslateBaseException(message)
{
}
