#pragma once
#include "Services\Translation\Dto\TranslateResultCategory.h"
#include "Services\Translation\Dto\TranslateResultSentence.h"
#include "json.hpp"

using namespace nlohmann;

class TranslateResult
{
private:
    bool isEmptyResult;
    TranslateResultSentence sentence;
    vector<TranslateResultCategory> translateCategories;

    static TranslateResultSentence ParseTranslateResultSentence(nlohmann::json root);
    static vector<TranslateResultCategory> ParseTranslateCategories(nlohmann::json root);

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