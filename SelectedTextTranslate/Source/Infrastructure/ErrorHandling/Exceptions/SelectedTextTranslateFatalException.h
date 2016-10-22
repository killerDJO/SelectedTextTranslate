#pragma once
#include "Infrastructure\ErrorHandling\Exceptions\Base\SelectedTextTranslateBaseException.h"

class SelectedTextTranslateFatalException : public SelectedTextTranslateBaseException
{
public:
    SelectedTextTranslateFatalException(wstring message);
};