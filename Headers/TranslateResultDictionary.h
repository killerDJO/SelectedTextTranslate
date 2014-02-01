#pragma once
#include "stdafx.h"
#include "TranslateResultDictionaryEntry.h"

struct TranslateResultDictionary
{
	wchar_t* PartOfSpeech;
	wchar_t* BaseForm;
	vector<TranslateResultDictionaryEntry> Entries;
};