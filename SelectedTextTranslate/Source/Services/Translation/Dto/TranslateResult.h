#pragma once
#include "Services\Translation\Dto\TranslateResultCategory.h"
#include "Services\Translation\Dto\TranslateResultSentence.h"
#include "json.hpp"

using namespace nlohmann;

class TranslateResult
{
private:
    static const char* SentenceKey;
    static const char* CategoriesKey;

    static const char* TranslationKey;
    static const char* OriginKey;
    static const char* InputKey;
    static const char* SuggestionKey;

    static const char* EntriesKey;
    static const char* BaseFormKey;
    static const char* PartOfSpeechKey;
    static const char* WordKey;
    static const char* ScoreKey;
    static const char* ReverseTranslationsKey;

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