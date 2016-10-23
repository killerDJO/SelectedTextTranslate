#pragma once
#include "Services\Translation\Dto\TranslateResultDictionary.h"
#include "Services\Translation\Dto\TranslateResultSentence.h"

class TranslateResult
{
public:
    TranslateResultSentence Sentence;
    vector<TranslateResultDictionary> TranslateCategories;

    TranslateResult();

    bool IsInputCorrected() const;
};