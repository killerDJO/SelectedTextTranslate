#include "PrecompiledHeaders\stdafx.h"
#include "TranslateEngine\Translator.h"
#include "TranslateEngine\Logger.h"

time_t Translator::lastTkkRequestTime = -1;
int Translator::tkk1 = 0;
int Translator::tkk2 = 0;

TranslateResult Translator::TranslateSelectedText()
{
	string selectedText = TextExtractor::GetSelectedText();
	
	Logger::AddRecord(selectedText);

	return TranslateSentence(selectedText);
}

TranslateResult Translator::TranslateSentence(string sentence)
{	
	UpateTkkIfNeccessary();
	string hash = GetHash(sentence, tkk1, tkk2);
	string translateURL = "https://translate.google.com/translate_a/single?client=t&sl=en&tl=ru&hl=ru&dt=at&dt=bd&dt=ex&dt=ld&dt=md&dt=qca&dt=rw&dt=rm&dt=ss&dt=t&ie=UTF-8&oe=UTF-8&source=bh&ssel=0&tsel=0&kc=1&tco=2&tk=" + hash + "&q=" + RequestHelper::EscapeText(sentence);
	string translatorResponse = RequestHelper::GetResponse(translateURL);
	return ParseJSONResponse(translatorResponse);
}

void Translator::UpateTkkIfNeccessary()
{
	time_t  timev;
	time(&timev);

	if (Translator::lastTkkRequestTime == -1 || timev - Translator::lastTkkRequestTime > 60 * 60)
	{
		string translatePageURL = "https://translate.google.com";
		string translatePageMarkup = RequestHelper::GetResponse(translatePageURL);

		GumboOutput* output = gumbo_parse(translatePageMarkup.c_str());
		string scriptContent = SearchScriptTag(output->root);

		if (scriptContent == "")
		{
			return;
		}

		duk_context *ctx = duk_create_heap_default();
		duk_push_c_function(ctx, ExtractTKK, DUK_VARARGS);
		duk_put_global_string(ctx, "getTKK");

		scriptContent += "; getTKK(TKK);";

		duk_eval_string_noresult(ctx, scriptContent.c_str());

		duk_destroy_heap(ctx);

		Translator::lastTkkRequestTime = timev;
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

	Translator::tkk1 = atoi(parts[0].c_str());
	Translator::tkk2 = atoi(parts[1].c_str());

	return 0;
}

// Grabbed from google code
string Translator::GetHash(string sentence, int tkk1, int tkk2)
{
	const char* bytes = sentence.c_str();

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

	return to_string(a) + "." + to_string(a ^ tkk1);
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

	if (json.empty()){
		result.Sentence.Translation = L"[Error]";
		result.Sentence.Origin = L"[Error]";
		return result;
	}

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

vector<string> Translator::Split(const string &s, char delim) {
	vector<string> elems;

	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}