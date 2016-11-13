#pragma once
#include "Services\Translation\Dto\TranslateResultDictionaryEntry.h"

struct TranslateResultDictionary
{
    wstring PartOfSpeech;
    wstring BaseForm;
    bool IsExtendedList;
    vector<TranslateResultDictionaryEntry> Entries;
};