#include "Services\Translation\TranslationService.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateException.h"
#include "Utilities\StringUtilities.h"

using namespace web;

TranslationService::TranslationService(Logger* logger, RequestProvider* requestProvider, TranslatePageParser* translatePageParser, DictionaryService* dictionary)
{
    this->requestProvider = requestProvider;
    this->translatePageParser = translatePageParser;
    this->logger = logger;
    this->dictionaryService = dictionary;
}

TranslateResult TranslationService::TranslateSentence(wstring sentence, bool incrementTranslationsCount, bool forceTranslation) const
{
    wstring trimmedSentence = StringUtilities::Trim(sentence);
    logger->Log(L"Start translating sentence '" + trimmedSentence + L"'.");

    wstring translatorResponse = GetTranslatorResponse(trimmedSentence, incrementTranslationsCount, forceTranslation);

    TranslateResult result;

    try
    {
        result = ParseJSONResponse(translatorResponse);
        result.Sentence.Input = StringUtilities::CopyWideChar(trimmedSentence);
    }
    catch (json::json_exception exception)
    {
        throw SelectedTextTranslateException(L"Error parsing json response. Exception: " + StringUtilities::GetUtf16String(exception.what()) + L".");
    }

    logger->Log(L"End translating sentence.");

    return result;
}

wstring TranslationService::GetTranslatorResponse(wstring sentence, bool incrementTranslationsCount, bool forceTranslation) const
{
    wstring translatorResponse;

    LogRecord cachedRecord;
    if(dictionaryService->TryGetCachedRecord(sentence, forceTranslation, cachedRecord))
    {
        time_t currentTime = time(nullptr);

        if(currentTime - cachedRecord.UpdatedDate > DictionaryRefreshInterval)
        {
            translatorResponse = SendTranslationRequest(sentence, forceTranslation);
            dictionaryService->UpdateTranslateResult(sentence, translatorResponse, forceTranslation);
        }
        else
        {
            translatorResponse = cachedRecord.Json;
        }
    }
    else
    {
        translatorResponse = SendTranslationRequest(sentence, forceTranslation);
        dictionaryService->AddTranslateResult(sentence, translatorResponse, forceTranslation);
    }

    if (incrementTranslationsCount)
    {
        dictionaryService->IncrementTranslationsCount(sentence, forceTranslation);
    }

    return translatorResponse;
}

wstring TranslationService::SendTranslationRequest(wstring sentence, bool forceTranslation) const
{
    wstring hash = GetHash(sentence);
    wstring translateURL = StringUtilities::Format(
        L"https://translate.google.com/translate_a/single?client=t&sl=en&tl=ru&hl=ru&dt=at&dt=bd&dt=ex&dt=ld&dt=md&dt=%ls&dt=rw&dt=rm&dt=ss&dt=t&ie=UTF-8&oe=UTF-8&source=bh&ssel=0&tsel=0&kc=1&tco=2&tk=%ls&q=%ls",
        forceTranslation ? L"qc" : L"qca",
        hash.c_str(),
        requestProvider->EscapeText(sentence).c_str());

    return requestProvider->GetStringResponse(translateURL);
}

// Grabbed from google minified JS code
wstring TranslationService::GetHash(wstring sentence) const
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
//  [
//      ["<translation>", "<original text>"],
//      [null, null, "<translit>"]
//  ],
//  [
//      [
//          "<part_of_speech>",
//          [<irrelevant>],
//          [
//              ["<variant_1>", ["<reverse_translation_1>", "<reverse_translation_2>", ..], null, <score>],
//              ....
//          ],
//          "<base_form>",
//          <irrelevant>
//      ],
//  ]
//]
TranslateResult TranslationService::ParseJSONResponse(wstring json) const
{
    TranslateResult result;

    if (json.empty())
    {
        throw SelectedTextTranslateException(L"Error. Unable to parse JSON. JSON value is empty.");
    }

    // Normalize json response
    StringUtilities::ReplaceAll(json, L",,", L",null,");
    StringUtilities::ReplaceAll(json, L"[,", L"[null,");

    json::value root = json::value::parse(json);

    if (root.is_null())
    {
        throw SelectedTextTranslateException(StringUtilities::Format(L"Error. Unable to parse JSON. Json value = '%ls'.", json));
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

TranslationService::~TranslationService()
{
}
