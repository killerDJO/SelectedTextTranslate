#include "BusinessLogic\Translation\TranslationService.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateException.h"
#include "Utilities\StringUtilities.h"

TranslationService::TranslationService(CompositionRoot* root)
{
    this->requestProvider = root->GetService<RequestProvider>();
    this->translatePageParser = root->GetService<TranslatePageParser>();
    this->logger = root->GetService<Logger>();
    this->dictionaryService = root->GetService<DictionaryService>();
}

TranslateResult TranslationService::TranslateSentence(wstring sentence, bool incrementTranslationsCount, bool forceTranslation) const
{
    wstring trimmedSentence = StringUtilities::Trim(sentence);

    if(trimmedSentence.empty())
    {
        return TranslateResult();
    }

    logger->Log(LogLevels::Trace, L"Start translating sentence '" + trimmedSentence + L"'.");

    TranslateResult result;
    try
    {
        result = GetTranslatorResponse(trimmedSentence, incrementTranslationsCount, forceTranslation);
    }
    catch (exception exception)
    {
        throw SelectedTextTranslateException(L"Error calling traslation service. Exception: " + StringUtilities::GetUtf16String(exception.what()) + L".");
    }

    logger->Log(LogLevels::Trace, L"End translating sentence.");

    return result;
}

TranslateResult TranslationService::GetTranslatorResponse(wstring sentence, bool incrementTranslationsCount, bool forceTranslation) const
{
    TranslateResult result;

    DictionaryRecord cachedRecord;
    if(dictionaryService->TryGetCachedRecord(sentence, forceTranslation, cachedRecord))
    {
        time_t currentTime = time(nullptr);
        if(currentTime - cachedRecord.GetUpdatedDate() > DictionaryRefreshInterval)
        {
            result = SendRequestAndParse(
                sentence,
                forceTranslation,
                bind(&DictionaryService::UpdateTranslateResult, dictionaryService, placeholders::_1, placeholders::_2, placeholders::_3));
        }
        else
        {
            result = TranslateResult::ParseFromJson(cachedRecord.GetJson());
        }
    }
    else
    {
        result = SendRequestAndParse(
            sentence,
            forceTranslation,
            bind(&DictionaryService::AddTranslateResult, dictionaryService, placeholders::_1, placeholders::_2, placeholders::_3));
    }

    if (incrementTranslationsCount)
    {
        dictionaryService->IncrementTranslationsCount(sentence, forceTranslation);
    }

    return result;
}

TranslateResult TranslationService::SendRequestAndParse(wstring sentence, bool forceTranslation, function<void(wstring, wstring, bool)> dictionaryAction) const
{
    wstring translatorResponse = SendTranslationRequest(sentence, forceTranslation);
    TranslateResult result = ParseJSONResponse(translatorResponse, sentence);

    dictionaryAction(sentence, TranslateResult::SerializeToJson(result), forceTranslation);

    return result;
}

wstring TranslationService::SendTranslationRequest(wstring sentence, bool forceTranslation) const
{
    wstring hash = GetHash(sentence);
    const wstring translateURL = StringUtilities::Format(
        L"translate_a/single?client=t&sl=en&tl=ru&hl=ru&dt=at&dt=bd&dt=ex&dt=ld&dt=md&dt=%ls&dt=rw&dt=rm&dt=ss&dt=t&ie=UTF-8&oe=UTF-8&source=bh&ssel=0&tsel=0&kc=1&tco=2&tk=%ls&q=%ls",
        forceTranslation ? L"qc" : L"qca",
        hash.c_str(),
        requestProvider->EscapeText(sentence).c_str());

    return requestProvider->GetStringResponse(L"translate.google.com", translateURL);
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
//  ],
//  <irrelevant>,
//  <irrelevant>,
//  <irrelevant>,
//  <irrelevant>,
//  <irrelevant>,
//  [<irrelevant>, <suggestion>, <irrelevant>, null, null, <irrelevant>],
//]
TranslateResult TranslationService::ParseJSONResponse(wstring jsonResponse, wstring input) const
{
    if (jsonResponse.empty())
    {
        throw SelectedTextTranslateException(L"Error. Unable to parse JSON. JSON value is empty.");
    }

    // Normalize json response
    StringUtilities::ReplaceAll(jsonResponse, L",,", L",null,");
    StringUtilities::ReplaceAll(jsonResponse, L"[,", L"[null,");

    json root = json::parse(StringUtilities::GetUtf8String(jsonResponse).c_str());

    if (root.is_null())
    {
        throw SelectedTextTranslateException(StringUtilities::Format(L"Error. Unable to parse JSON. Json value = '%ls'.", jsonResponse));
    }

    TranslateResultSentence sentence = ParseTranslateResultSentence(root, input);

    if (!root[1].is_array())
    {
        return TranslateResult(sentence, vector<TranslateResultCategory>());
    }

    vector<TranslateResultCategory> categories = ParseTranslateCategories(root);

    return TranslateResult(sentence, categories);
}

TranslateResultSentence TranslationService::ParseTranslateResultSentence(json root, wstring input) const
{
    wstring translation;
    wstring origin;
    json sentences = root[0][0];
    if (sentences.size() > 0)
    {
        translation = StringUtilities::GetJsonString(sentences[0]);
        origin = StringUtilities::GetJsonString(sentences[1]);
    }

    wstring suggestion;
    if (!root[7].is_null())
    {
        json suggestionHolder = root[7];
        suggestion = StringUtilities::GetJsonString(suggestionHolder[1]);
    }

    return TranslateResultSentence(translation, origin, input, suggestion);
}

vector<TranslateResultCategory> TranslationService::ParseTranslateCategories(json root) const
{
    vector<TranslateResultCategory> categories;

    json dict = root[1];

    for (size_t i = 0; i < dict.size(); ++i)
    {
        wstring partOfSpeech = StringUtilities::GetJsonString(dict[i][0]);
        wstring baseForm = StringUtilities::GetJsonString(dict[i][3]);

        if (!dict[i][2].is_array())
        {
            continue;
        }

        vector<TranslateResultCategoryEntry> translateResultDictionaryEntries;
        json entries = dict[i][2];
        for (size_t j = 0; j < entries.size(); ++j)
        {
            wstring word = StringUtilities::GetJsonString(entries[j][0]);

            double score = entries[j][3].is_number_float()
                ? entries[j][3].get<double>()
                : 0;

            if (!entries[j][1].is_array())
            {
                continue;
            }

            vector<wstring> reverseTransltions;
            json reverseTranslationsJson = entries[j][1];
            for (size_t k = 0; k < reverseTranslationsJson.size(); ++k)
            {
                reverseTransltions.push_back(StringUtilities::GetJsonString(reverseTranslationsJson[k]));
            }

            translateResultDictionaryEntries.push_back(TranslateResultCategoryEntry(word, reverseTransltions, score));
        }

        categories.push_back(TranslateResultCategory(partOfSpeech, baseForm, false, translateResultDictionaryEntries));
    }

    return categories;
}