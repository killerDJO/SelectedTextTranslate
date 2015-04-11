#include "PrecompiledHeaders\stdafx.h"
#include "TranslateEngine\Translator.h"

TranslateResult Translator::TranslateSelectedText()
{
	string selectedText = TextExtractor::GetSelectedText();
	return TranslateSentence(selectedText);
}

TranslateResult Translator::TranslateSentence(string sentence)
{
	string translateURL = "http://translate.google.com/translate_a/single?client=t&sl=en&tl=ru&hl=ru&dt=bd&dt=ex&dt=ld&dt=md&dt=qca&dt=rw&dt=rm&dt=ss&dt=t&dt=at&ie=UTF-8&oe=UTF-8&otf=2&srcrom=1&ssel=0&tsel=0&q=" + RequestHelper::EscapeText(sentence);
	string translatorResponse = RequestHelper::GetResponse(translateURL);
	return ParseJSONResponse(translatorResponse);
}

// Relevant response has the following format
//[
//	[
//		["<translation>", "<original text>"],
//		[null, null, "<translit>"]
//	],
//	[
//		[
//			"<part_of_speech>", 
//			[<irrelevant>],
//			[
//				["<variant_1>", ["<reverse_translation_1>", "<reverse_translation_2>", ..], null, <score>],
//				....
//			], 
//			"<base_form>", 
//			<irrelevant>
//		],		
//	]
//]
TranslateResult Translator::ParseJSONResponse(string json)
{
	Json::Value root;
	Json::Reader reader;
	TranslateResult result;

	// Normalize json response
	Translator::ReplaceAll(json, ",,", ",null,");
	Translator::ReplaceAll(json, "[,", "[null,");

	bool parsingSuccessful = reader.parse(json, root);
	if (!parsingSuccessful)
	{
		return result;
	}

	Json::Value sentences = root[0u].get(0u,"[]");
	if (sentences.size() > 0)
	{
		result.Sentence.Translation = Utilities::GetWideChar(sentences.get(0u, "").asString());
		result.Sentence.Origin = Utilities::GetWideChar(sentences.get(1u, "").asString());
	}

	Json::Value dict = root.get(1u, "[]");
	for (size_t i = 0; i < dict.size(); ++i)
	{
		TranslateResultDictionary category;
		category.IsExtendedList = false;
		category.PartOfSpeech = Utilities::GetWideChar(dict[i].get(0u, "").asString());
		category.BaseForm = Utilities::GetWideChar(dict[i].get(3u, "").asString());
		Json::Value entries = dict[i].get(2u, "[]");
		for (size_t j = 0; j < entries.size(); ++j)
		{
			TranslateResultDictionaryEntry entry;
			entry.Word = Utilities::GetWideChar(entries[j].get(0u, "").asString());
			entry.Score = entries[j].get(3u, 0.0).asDouble();

			auto reverseTranslations = entries[j].get(1u, "[]");
			for (size_t k = 0; k < reverseTranslations.size(); ++k)
			{
				entry.ReverseTranslation.push_back(Utilities::GetWideChar(reverseTranslations[k].asString()));
			}

			category.Entries.push_back(entry);
		}
		result.TranslateCategories.push_back(category);
	}
	return result;
}

void Translator::ReplaceAll(string &str, const string &search, const string &replace)
{
	for (size_t pos = 0;; pos += replace.length() - 1)
	{
		pos = str.find(search, pos);
		if (pos == string::npos) break;

		str.erase(pos, search.length());
		str.insert(pos, replace);
	}
}