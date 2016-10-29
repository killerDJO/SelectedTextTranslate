#pragma once
#include "Services\Translation\Dto\TranslateResultCategoryEntry.h"

class TranslateResultCategory
{
private:
    wstring partOfSpeech;
    wstring baseForm;
    bool isExtendedList;
    vector<TranslateResultCategoryEntry> entries;

public:
    TranslateResultCategory(wstring partOfSpeech, wstring baseForm, bool isExtendedList, vector<TranslateResultCategoryEntry> entries);
    ~TranslateResultCategory();

    wstring GetPartOfSpeech() const;
    wstring GetBaseForm() const;
    bool IsExtendedList() const;
    vector<TranslateResultCategoryEntry> GetEntries() const;

    void Toggle();
};