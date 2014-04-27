#pragma once
#include "PrecompiledHeaders\stdafx.h"
#include "Entities\TranslateResultDictionaryEntry.h"

struct TranslateResultDictionary
{
	wchar_t* PartOfSpeech;
	wchar_t* BaseForm;
	bool IsExtendedList;
	vector<TranslateResultDictionaryEntry> Entries;
};