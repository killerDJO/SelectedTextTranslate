#pragma once
#include "Service\Translation\Dto\TranslateResultDictionary.h"
#include "Service\Translation\Dto\TranslateResultSentence.h"
#include "Helpers\StringUtilities.h"

class TranslateResult
{
private:
    bool isErrorResult;

public:
    TranslateResultSentence Sentence;
    vector<TranslateResultDictionary> TranslateCategories;

    wchar_t* ErrorMessage;

    TranslateResult();

    bool IsInputCorrected() const;

    void Free();
};