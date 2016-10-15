#pragma once
#include "Exceptions\Base\SelectedTextTranslateBaseException.h"

class SelectedTextTranslateException : public SelectedTextTranslateBaseException
{
public:
    SelectedTextTranslateException(wstring message, const wchar_t* file, unsigned int line);
};