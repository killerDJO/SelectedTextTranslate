#include "Services\Translation\Dto\TranslateResultCategory.h"

TranslateResultCategory::TranslateResultCategory(wstring partOfSpeech, wstring baseForm, bool isExtendedList, vector<TranslateResultCategoryEntry> entries)
{
    this->partOfSpeech = partOfSpeech;
    this->baseForm = baseForm;
    this->isExtendedList = isExtendedList;
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

bool TranslateResultCategory::IsExtendedList() const
{
    return isExtendedList;
}

vector<TranslateResultCategoryEntry> TranslateResultCategory::GetEntries() const
{
    return entries;
}

void TranslateResultCategory::Toggle()
{
    isExtendedList ^= true;;
}

TranslateResultCategory::~TranslateResultCategory()
{
}
