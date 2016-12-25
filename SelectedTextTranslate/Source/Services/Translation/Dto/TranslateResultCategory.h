#pragma once
#include "Services\Translation\Dto\TranslateResultCategoryEntry.h"

class TranslateResultCategory
{
private:
    wstring partOfSpeech;
    wstring baseForm;
    vector<TranslateResultCategoryEntry> entries;

public:
    TranslateResultCategory(wstring partOfSpeech, wstring baseForm, bool isExtendedList, vector<TranslateResultCategoryEntry> entries);

    wstring GetPartOfSpeech() const;
    wstring GetBaseForm() const;
    vector<TranslateResultCategoryEntry> GetEntries() const;
};