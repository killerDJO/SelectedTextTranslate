#pragma once
#include "PrecompiledHeaders\stdafx.h"
#include "Entities\TranslateResultDictionaryEntry.h"

struct TranslateResultDictionary
{
	const wchar_t* PartOfSpeech;
	const wchar_t* BaseForm;
	bool IsExtendedList;
	vector<TranslateResultDictionaryEntry> Entries;
};