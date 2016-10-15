#include "Service\Translation\Translator.h"
#include "Helpers\StringUtilities.h"
#include "Exceptions\SelectedTextTranslateException.h"

using namespace web;

Translator::Translator(Logger* logger, RequestProvider* requestProvider, TranslatePageParser* translatePageParser, Dictionary* dictionary)
{
    this->requestProvider = requestProvider;
    this->translatePageParser = translatePageParser;
    this->logger = logger;
    this->dictionary = dictionary;
}

TranslateResult Translator::TranslateSentence(wstring sentence, bool updateDictionary) const
{
    logger->Log(L"Start translating sentence '" + sentence + L"'.");

    vector<LogRecord> dictionaryRecords = dictionary->GetRecords(sentence);

    wstring translatorResponse;

    if(dictionaryRecords.size() != 0)
    {
        translatorResponse = dictionaryRecords[0].Json;

        if (updateDictionary)
        {
            dictionary->UpdateTranslateResult(sentence);
        }
    }
    else
    {
        wstring hash = GetHash(sentence);
        wstring translateURL = L"https://translate.google.com/translate_a/single?client=t&sl=en&tl=ru&hl=ru&dt=at&dt=bd&dt=ex&dt=ld&dt=md&dt=qca&dt=rw&dt=rm&dt=ss&dt=t&ie=UTF-8&oe=UTF-8&source=bh&ssel=0&tsel=0&kc=1&tco=2&tk="
            + hash
            + L"&q=" + requestProvider->EscapeText(sentence);

        translatorResponse = requestProvider->GetStringResponse(translateURL);

        if (updateDictionary)
        {
            dictionary->AddTranslateResult(sentence, translatorResponse);
        }
    }

    TranslateResult result;

    try
    {
        result = ParseJSONResponse(translatorResponse);
        result.Sentence.Input = StringUtilities::CopyWideChar(sentence);
    }
    catch (json::json_exception exception)
    {
        throw SelectedTextTranslateException(L"Error parsing json response. Exception: " + StringUtilities::GetUtf16String(exception.what()) + L".", __WFILE__, __LINE__);
    }

    logger->Log(L"End translating sentence.");

    return result;
}

// Grabbed from google minified JS code
wstring Translator::GetHash(wstring sentence) const
{
    string utf8Sentence = StringUtilities::GetUtf8String(sentence);
    const char* bytes = utf8Sentence.c_str();

    long long tkk1 = translatePageParser->GetTkk1();
    long long tkk2 = translatePageParser->GetTkk2();

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
TranslateResult Translator::ParseJSONResponse(wstring json) const
{
    TranslateResult result;

    if (json.empty())
    {
        throw SelectedTextTranslateException(L"Error. Unable to parse JSON. JSON value is empty.", __WFILE__, __LINE__);
    }

    // Normalize json response
    ReplaceAll(json, L",,", L",null,");
    ReplaceAll(json, L"[,", L"[null,");

    json::value root = json::value::parse(json);

    if (root.is_null())
    {
        throw SelectedTextTranslateException(StringUtilities::Format(L"Error. Unable to parse JSON. Json value = '%ls'.", json), __WFILE__, __LINE__);
    }

    json::array sentences = root[0][0].as_array();
    if (sentences.size() > 0)
    {
        result.Sentence.Translation = StringUtilities::CopyWideChar(sentences[0].as_string());
        result.Sentence.Origin = StringUtilities::CopyWideChar(sentences[1].as_string());
    }

    if (!root[1].is_array()) {
        return result;
    }

    json::array dict = root[1].as_array();

    for (size_t i = 0; i < dict.size(); ++i)
    {
        TranslateResultDictionary category;

        category.IsExtendedList = false;
        category.PartOfSpeech = StringUtilities::CopyWideChar(dict[i][0].as_string());
        category.BaseForm = StringUtilities::CopyWideChar(dict[i][3].as_string());

        if (!dict[i][2].is_array()) 
        {
            continue;
        }

        json::array entries = dict[i][2].as_array();
        for (size_t j = 0; j < entries.size(); ++j)
        {
            TranslateResultDictionaryEntry entry;
            entry.Word = StringUtilities::CopyWideChar(entries[j][0].as_string());

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
                entry.ReverseTranslation.push_back(StringUtilities::CopyWideChar(reverseTranslations[k].as_string()));
            }

            category.Entries.push_back(entry);
        }

        result.TranslateCategories.push_back(category);
    }

    return result;
}

void Translator::ReplaceAll(wstring &str, const wstring &search, const wstring &replace) const
{
    for (size_t pos = 0;; pos += replace.length() - 1)
    {
        pos = str.find(search, pos);
        if (pos == string::npos) break;

        str.erase(pos, search.length());
        str.insert(pos, replace);
    }
}

Translator::~Translator()
{
}