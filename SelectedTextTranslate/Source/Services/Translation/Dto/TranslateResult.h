#pragma once
#include "Services\Translation\Dto\TranslateResultCategory.h"
#include "Services\Translation\Dto\TranslateResultSentence.h"

class TranslateResult
{
private:
    bool isEmptyResult;
    TranslateResultSentence sentence;
    vector<TranslateResultCategory> translateCategories;

public:
    TranslateResult(TranslateResultSentence sentence, vector<TranslateResultCategory> translateCategories);
    TranslateResult();
    ~TranslateResult();

    bool IsEmptyResult() const;
    TranslateResultSentence GetSentence() const;
    vector<TranslateResultCategory> GetTranslateCategories() const;
    bool IsInputCorrected() const;

    void ToggleCategory(int translateResultCategoryIndex);
};