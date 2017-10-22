#include "BusinessLogic\Translation\Dto\TranslateResultCategory.h"

TranslateResultCategory::TranslateResultCategory(wstring partOfSpeech, wstring baseForm, bool isExtendedList, vector<TranslateResultCategoryEntry> entries)
{
    this->partOfSpeech = partOfSpeech;
    this->baseForm = baseForm;
    this->entries = entries;
}

wstring TranslateResultCategory::GetPartOfSpeech() const
{
    return partOfSpeech;
}

wstring TranslateResultCategory::GetBaseForm() const
{
    return baseForm;
}

vector<TranslateResultCategoryEntry> TranslateResultCategory::GetEntries() const
{
    return entries;
}