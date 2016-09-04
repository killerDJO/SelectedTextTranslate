#pragma once
#include "Entities\TranslateResultDictionary.h"
#include "Entities\TranslateResultSentence.h"
#include "Helpers\StringUtilities.h"

class TranslateResult
{
public:
    TranslateResultSentence Sentence;
    vector<TranslateResultDictionary> TranslateCategories;
    
    wchar_t* ErrorMessage;
    
    TranslateResult();
    
    bool IsInputCorrected();
    void SetError(wstring errorMessage);
    bool IsErrorResult();
    
    void Free();

private:
    bool isErrorResult;
};