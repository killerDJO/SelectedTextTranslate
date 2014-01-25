#pragma once
#include "stdafx.h"
#include "TranslateResultDictionaryEntry.h"

struct TranslateResultDictionary
{
	string PartOfSpeech;
	vector<TranslateResultDictionaryEntry> Entries;
};