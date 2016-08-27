#include "PrecompiledHeaders\stdafx.h"
#include "TranslateEngine\Translator.h"
#include "Loggers\DictionaryLogger.h"
#include "Loggers\Logger.h"

using namespace web;

TranslateResult Translator::TranslateSelectedText()
{
	wstring selectedText = TextExtractor::GetSelectedText();
	
	DictionaryLogger::AddRecord(selectedText);

	return TranslateSentence(selectedText);
}

TranslateResult Translator::TranslateSentence(wstring sentence)
{	
	Logger::Log(L"Start translating sentence '" + sentence + L"'.");

	wstring hash = GetHash(sentence);
	wstring translateURL = L"https://translate.google.com/translate_a/single?client=t&sl=en&tl=ru&hl=ru&dt=at&dt=bd&dt=ex&dt=ld&dt=md&dt=qca&dt=rw&dt=rm&dt=ss&dt=t&ie=UTF-8&oe=UTF-8&source=bh&ssel=0&tsel=0&kc=1&tco=2&tk=" + hash + L"&q=" + RequestHelper::EscapeText(sentence);
	wstring translatorResponse = RequestHelper::GetStringResponse(translateURL);

	TranslateResult result;

	try 
	{
		result = ParseJSONResponse(translatorResponse);
	}
	catch (json::json_exception exception)
	{
		Logger::Log(L"Error parsing json response. Exception: " + Utilities::GetUtf16String(exception.what()) + L".");
	}

	Logger::Log(L"End translating sentence.");

	return result;
}

// Grabbed from google minified js code
wstring Translator::GetHash(wstring sentence)
{
	string utf8Sentence = Utilities::GetUtf8String(sentence);
	const char* bytes = utf8Sentence.c_str();

	long long tkk1 = PageParser::GetTkk1();
	long long tkk2 = PageParser::GetTkk2();

	long long a = tkk1;
	long long pow32 = 4294967295;
	long long pow31 = pow32 / 2;

	for (unsigned i = 0; i < strlen(bytes); ++i) {
		a = a + (unsigned char)bytes[i];

		/* ------ */
		a = a + (a << 10) & pow32;
		a = a ^ (unsigned(a) >> 6);
		/* ------ */
	}

	/* ------ */
	a = a + (a << 3) & pow32;
	a = a ^ (unsigned(a) >> 11);
	a = a + (a << 15) & pow32;
	/* ------ */

	a = a ^ tkk2;

	if (a < 0) {
		a = a & pow31 + pow31 + 1;
	}

	a = a % 1000000;

	return to_wstring(a) + L"." + to_wstring(a ^ tkk1);
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
TranslateResult Translator::ParseJSONResponse(wstring json)
{
	TranslateResult result;

	if (json.empty()){
		result.Sentence.Translation = Utilities::CopyWideChar(L"[Error]");
		result.Sentence.Origin = Utilities::CopyWideChar(L"[Error]");

		Logger::Log(L"Error. Unable to parse JSON. JSON value is empty.");

		return result;
	}

	// Normalize json response
	Translator::ReplaceAll(json, L",,", L",null,");
	Translator::ReplaceAll(json, L"[,", L"[null,");

	json::value root = json::value::parse(json);

	if (root.is_null())
	{
		Logger::Log(L"Error. Unable to parse JSON. Json value = '" + json + L"'.");
		result.Sentence.Translation = Utilities::CopyWideChar(L"[Error]");
		result.Sentence.Origin = Utilities::CopyWideChar(L"[Error]");

		return result;
	}

	json::array sentences = root[0][0].as_array();
	if (sentences.size() > 0)
	{
		result.Sentence.Translation = Utilities::CopyWideChar(sentences[0].as_string());
		result.Sentence.Origin = Utilities::CopyWideChar(sentences[1].as_string());
	}

	if (!root[1].is_array()) {
		return result;
	}

	json::array dict = root[1].as_array();

	for (size_t i = 0; i < dict.size(); ++i)
	{
		TranslateResultDictionary category;

		category.IsExtendedList = false;
		category.PartOfSpeech = Utilities::CopyWideChar(dict[i][0].as_string());
		category.BaseForm = Utilities::CopyWideChar(dict[i][3].as_string());

		if (!dict[i][2].is_array()) 
		{
			continue;
		}

		json::array entries = dict[i][2].as_array();
		for (size_t j = 0; j < entries.size(); ++j)
		{
			TranslateResultDictionaryEntry entry;
			entry.Word = Utilities::CopyWideChar(entries[j][0].as_string());

			entry.Score = entries[j][3].is_double()
				? entries[j][3].as_double()
				: 0;

			if (!entries[j][1].is_array())
			{
				continue;
			}

			json::array reverseTranslations = entries[j][1].as_array();
			for (size_t k = 0; k < reverseTranslations.size(); ++k)
			{
				entry.ReverseTranslation.push_back(Utilities::CopyWideChar(reverseTranslations[k].as_string()));
			}

			category.Entries.push_back(entry);
		}
		result.TranslateCategories.push_back(category);
	}

	return result;
}

void Translator::ReplaceAll(wstring &str, const wstring &search, const wstring &replace)
{
	for (size_t pos = 0;; pos += replace.length() - 1)
	{
		pos = str.find(search, pos);
		if (pos == string::npos) break;

		str.erase(pos, search.length());
		str.insert(pos, replace);
	}
}