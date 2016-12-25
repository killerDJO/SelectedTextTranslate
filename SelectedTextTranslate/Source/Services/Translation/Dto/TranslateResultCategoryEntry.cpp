#include "Services\Translation\Dto\TranslateResultCategoryEntry.h"

TranslateResultCategoryEntry::TranslateResultCategoryEntry(wstring word, vector<wstring> reverseTranslations, double score)
{
    this->word = word;
    this->reverseTranslations = reverseTranslations;
    this->score = score;
}

TranslateResultCategoryEntry::TranslateResultCategoryEntry()
{
    this->word = wstring();
    this->reverseTranslations = vector<wstring>();
    this->score = 0;
}

wstring TranslateResultCategoryEntry::GetWord() const
{
    return word;
}

vector<wstring> TranslateResultCategoryEntry::GetReverseTranslations() const
{
    return reverseTranslations;
}

double TranslateResultCategoryEntry::GetScore() const
{
    return score;
}