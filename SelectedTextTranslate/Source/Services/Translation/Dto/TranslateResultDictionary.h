#pragma once
#include "Services\Translation\Dto\TranslateResultDictionaryEntry.h"

struct TranslateResultDictionary
{
    const wchar_t* PartOfSpeech;
    const wchar_t* BaseForm;
    bool IsExtendedList;
    vector<TranslateResultDictionaryEntry> Entries;
};