#pragma once
#include "Exceptions\Base\SelectedTextTranslateBaseException.h"

class SelectedTextTranslateFatalException : public SelectedTextTranslateBaseException
{
public:
    SelectedTextTranslateFatalException(wstring message, const wchar_t* file, unsigned int line);
};