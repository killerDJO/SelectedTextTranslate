#pragma once
#include "Infrastructure\ErrorHandling\Exceptions\Base\SelectedTextTranslateBaseException.h"

class SelectedTextTranslateException : public SelectedTextTranslateBaseException
{
public:
    SelectedTextTranslateException(wstring message);
};