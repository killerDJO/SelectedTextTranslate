#pragma once

struct TranslateResultDictionaryEntry
{
	const wchar_t* Word;
	vector<const wchar_t*> ReverseTranslation;
	double Score;
};