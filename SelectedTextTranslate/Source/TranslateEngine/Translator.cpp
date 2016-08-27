#include "PrecompiledHeaders\stdafx.h"
#include "TranslateEngine\Translator.h"
#include "Loggers\DictionaryLogger.h"
#include "Loggers\Logger.h"

using namespace web;

time_t Translator::lastTkkRequestTime = -1;
long long Translator::tkk1 = 0;
long long Translator::tkk2 = 0;

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
	TranslateResult result = ParseJSONResponse(translatorResponse);

	Logger::Log(L"End translating sentence.");

	return result;
}

void Translator::UpateTkkIfNeccessary()
{
	time_t  timev;
	time(&timev);

	if (Translator::lastTkkRequestTime == -1 || timev - Translator::lastTkkRequestTime > 60 * 60)
	{
		Logger::Log(L"Start requesting TKK.");

		wstring translatePageURL = L"https://translate.google.com";
		wstring translatePageMarkup = RequestHelper::GetStringResponse(translatePageURL);

		GumboOutput* output = gumbo_parse(Utilities::GetUtf8String(translatePageMarkup).c_str());
		string scriptContent = SearchScriptTag(output->root);

		if (scriptContent == "")
		{
			Logger::Log(L"Error. Unable to find script with TKK eval.");
			return;
		}

		duk_context *ctx = duk_create_heap_default();
		duk_push_c_function(ctx, ExtractTKK, DUK_VARARGS);
		duk_put_global_string(ctx, "getTKK");

		scriptContent += "; getTKK(TKK);";

		duk_eval_string_noresult(ctx, scriptContent.c_str());

		duk_destroy_heap(ctx);

		Translator::lastTkkRequestTime = timev;

		Logger::Log(L"End requesting TKK.");
	}
}

string Translator::SearchScriptTag(GumboNode* node) {

	if (node->type != GUMBO_NODE_ELEMENT)
	{
		return "";
	}

	if (node->v.element.tag == GUMBO_TAG_SCRIPT)
	{
		string scriptContent = static_cast<GumboNode*>(node->v.element.children.data[0])->v.text.text;

		if (scriptContent.find("TKK=") != std::string::npos)
		{
			return scriptContent;
		}
	}

	GumboVector* children = &node->v.element.children;
	for (unsigned int i = 0; i < children->length; ++i)
	{
		string result = SearchScriptTag(static_cast<GumboNode*>(children->data[i]));

		if (result != "")
		{
			return result;
		}
	}

	return "";
}

duk_ret_t Translator::ExtractTKK(duk_context *ctx)
{
	const char* res = duk_to_string(ctx, 0);

	if (res == NULL)
	{
		return -1;
	}

	vector<string> parts = Split(res, '.');

	Translator::tkk1 = atoll(parts[0].c_str());
	Translator::tkk2 = atoll(parts[1].c_str());

	Logger::Log(L"TKK has been extracted. New values: Tkk1 = " + to_wstring(Translator::tkk1) + L", Tkk2 = " + to_wstring(Translator::tkk2) + L".");

	return 0;
}

// Grabbed from google minified js code
wstring Translator::GetHash(wstring sentence)
{
	UpateTkkIfNeccessary();

	string utf8Sentence = Utilities::GetUtf8String(sentence);
	const char* bytes = utf8Sentence.c_str();

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

vector<string> Translator::Split(const string &s, char delim) {
	vector<string> elems;

	stringstream ss(s);
	string item;
	while (getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}