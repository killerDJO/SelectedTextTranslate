#pragma once
#include "Services\Translation\Dto\TranslateResultCategory.h"
#include "Services\Translation\Dto\TranslateResultSentence.h"

using namespace web;

class TranslateResult
{
private:
    bool isEmptyResult;
    TranslateResultSentence sentence;
    vector<TranslateResultCategory> translateCategories;

    static TranslateResultSentence ParseTranslateResultSentence(json::value root);
    static vector<TranslateResultCategory> ParseTranslateCategories(json::value root);

public:
    TranslateResult(TranslateResultSentence sentence, vector<TranslateResultCategory> translateCategories);
    TranslateResult();
    ~TranslateResult();

    bool IsEmptyResult() const;
    TranslateResultSentence GetSentence() const;
    vector<TranslateResultCategory> GetTranslateCategories() const;
    bool IsInputCorrected() const;

    void ToggleCategory(int translateResultCategoryIndex);

    static wstring SerializeToJson(TranslateResult translateResult);
    static TranslateResult ParseFromJson(wstring json);
};