#include "stdafx.h"
#include "Translator.h"

TranslateResult Translator::TranslateSelectedText()
{
	string selectedText = TextExtractor::GetSelectedText();
	return TranslateSentence(selectedText);
}

TranslateResult Translator::TranslateSentence(string sentence)
{
	string translateURL = "http://translate.google.ru/translate_a/t?client=x&hl=en&sl=en&tl=ru&text=" + RequestHelper::EscapeText(sentence);
	string translatorResponse = RequestHelper::GetResponse(translateURL);
	return ParseJSONResponse(translatorResponse);
}

TranslateResult Translator::ParseJSONResponse(string json)
{
	Json::Value root;  
	Json::Reader reader;
	TranslateResult result;

	bool parsingSuccessful = reader.parse(json, root );
	if (!parsingSuccessful)
	{
		return result;;
	}

	Json::Value sentences = root.get("sentences","[]");
	if(sentences.size() > 0)
	{
		int idx = 0;
		result.Sentence.Translation = Utilities::GetWideChar(sentences[idx].get("trans", "").asString());
		result.Sentence.Origin = Utilities::GetWideChar(sentences[idx].get("orig", "").asString());
		result.Sentence.Translit = Utilities::GetWideChar(sentences[idx].get("translit", "").asString());
	}

	Json::Value dict = root.get("dict","[]");
	for(size_t i = 0; i < dict.size(); ++i)
	{
		TranslateResultDictionary category;
		category.PartOfSpeech = Utilities::GetWideChar(dict[i].get("pos", "").asString());
		category.BaseForm = Utilities::GetWideChar(dict[i].get("base_form", "").asString());
		Json::Value entries =dict[i].get("entry", "[]");
		for (size_t j = 0; j < entries.size(); ++j)
		{
			TranslateResultDictionaryEntry entry;
			entry.Word = Utilities::GetWideChar(entries[j].get("word", "").asString());
			entry.Score = entries[j].get("score", 0.0).asDouble();
			
			auto reverseTranslations = entries[j].get("reverse_translation", "[]");
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

