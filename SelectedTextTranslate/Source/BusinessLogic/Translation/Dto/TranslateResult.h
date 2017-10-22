#pragma once
#include "BusinessLogic\Translation\Dto\TranslateResultCategory.h"
#include "BusinessLogic\Translation\Dto\TranslateResultSentence.h"
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

    static TranslateResultSentence ParseTranslateResultSentence(json root);
    static vector<TranslateResultCategory> ParseTranslateCategories(json root);

public:
    TranslateResult(TranslateResultSentence sentence, vector<TranslateResultCategory> translateCategories);
    TranslateResult();

    bool IsEmptyResult() const;
    TranslateResultSentence GetSentence() const;
    vector<TranslateResultCategory> GetTranslateCategories() const;
    bool IsInputCorrected() const;

    static wstring SerializeToJson(TranslateResult translateResult);
    static TranslateResult ParseFromJson(wstring json);

    bool EqualTo(TranslateResult other) const;
};