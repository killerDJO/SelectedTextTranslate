#pragma once
#include "Service\Translation\Dto\TranslateResultDictionaryEntry.h"

struct TranslateResultDictionary
{
    const wchar_t* PartOfSpeech;
    const wchar_t* BaseForm;
    bool IsExtendedList;
    vector<TranslateResultDictionaryEntry> Entries;
};