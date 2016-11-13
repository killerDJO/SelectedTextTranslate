#include "Services\Translation\Dto\TranslateResult.h"
#include "Utilities\StringUtilities.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateException.h"

TranslateResult::TranslateResult(TranslateResultSentence sentence, vector<TranslateResultCategory> translateCategories)
{
    this->isEmptyResult = false;
    this->sentence = sentence;
    this->translateCategories = translateCategories;
}

TranslateResult::TranslateResult()
{
    this->isEmptyResult = true;

    this->sentence = TranslateResultSentence();
    this->translateCategories = vector<TranslateResultCategory>();
}

bool TranslateResult::IsEmptyResult() const
{
    return isEmptyResult;
}

TranslateResultSentence TranslateResult::GetSentence() const
{
    if(isEmptyResult)
    {
        throw SelectedTextTranslateException(L"Result is empty.");
    }

    return sentence;
}

vector<TranslateResultCategory> TranslateResult::GetTranslateCategories() const
{
    if (isEmptyResult)
    {
        throw SelectedTextTranslateException(L"Result is empty.");
    }

    return translateCategories;
}

bool TranslateResult::IsInputCorrected() const
{
    if (isEmptyResult)
    {
        throw SelectedTextTranslateException(L"Result is empty.");
    }

    wstring trimmedOrigin = StringUtilities::Trim(sentence.GetOrigin());
    wstring trimmedInput = StringUtilities::Trim(sentence.GetInput());
    return trimmedOrigin.compare(trimmedInput) != 0;
}

void TranslateResult::ToggleCategory(int translateResultCategoryIndex)
{
    translateCategories[translateResultCategoryIndex].Toggle();
}

wstring TranslateResult::SerializeToJson(TranslateResult translateResult)
{
    json::value sentenceJson;
    sentenceJson[L"translation"] = json::value::string(translateResult.GetSentence().GetTranslation());
    sentenceJson[L"origin"] = json::value::string(translateResult.GetSentence().GetOrigin());
    sentenceJson[L"input"] = json::value::string(translateResult.GetSentence().GetInput());
    sentenceJson[L"suggestion"] = json::value::string(translateResult.GetSentence().GetSuggestion());

    json::value categoriesJson;
    for (size_t i = 0; i < translateResult.GetTranslateCategories().size(); ++i)
    {
        TranslateResultCategory category = translateResult.GetTranslateCategories()[i];
        json::value categoryJson;
        categoryJson[L"baseForm"] = json::value::string(category.GetBaseForm());
        categoryJson[L"partOfSpeech"] = json::value::string(category.GetPartOfSpeech());

        json::value categoryEntriesJson;
        for (size_t j = 0; j < category.GetEntries().size(); ++j)
        {
            TranslateResultCategoryEntry categoryEntry = category.GetEntries()[j];
            json::value categoryEntryJson;
            categoryEntryJson[L"word"] = json::value::string(categoryEntry.GetWord());
            categoryEntryJson[L"score"] = json::value::number(categoryEntry.GetScore());

            json::value reverseTranslationsJson;
            for (size_t k = 0; k < categoryEntry.GetReverseTranslations().size(); ++k)
            {
                reverseTranslationsJson[k] = json::value::string(categoryEntry.GetReverseTranslations()[k]);
            }

            categoryEntryJson[L"reverseTranslations"] = reverseTranslationsJson;

            categoryEntriesJson[j] = categoryEntryJson;
        }

        categoryJson[L"entries"] = categoryEntriesJson;

        categoriesJson[i] = categoryJson;
    }

    json::value translateResultJson;
    translateResultJson[L"sentence"] = sentenceJson;
    translateResultJson[L"categories"] = categoriesJson;

    wstring json = translateResultJson.serialize();
    return json;
}

TranslateResult TranslateResult::ParseFromJson(wstring json)
{
    json::value root = json::value::parse(json);

    if (root.is_null())
    {
        return TranslateResult();
    }

    TranslateResultSentence sentence = ParseTranslateResultSentence(root);

    vector<TranslateResultCategory> categories = ParseTranslateCategories(root);

    return TranslateResult(sentence, categories);
}

TranslateResultSentence TranslateResult::ParseTranslateResultSentence(json::value root)
{
    json::value sentenceJson = root[L"sentence"];

    wstring translation = sentenceJson[L"translation"].as_string();
    wstring origin = sentenceJson[L"origin"].as_string();
    wstring input = sentenceJson[L"input"].as_string();
    wstring suggestion = sentenceJson[L"suggestion"].as_string();

    return TranslateResultSentence(translation, origin, input, suggestion);
}

vector<TranslateResultCategory> TranslateResult::ParseTranslateCategories(json::value root)
{
    vector<TranslateResultCategory> categories;

    if (!root[L"categories"].is_array())
    {
        return categories;
    }

    json::array categoriesJson = root[L"categories"].as_array();

    for (size_t i = 0; i < categoriesJson.size(); ++i)
    {
        json::value categoryJson = categoriesJson[i];

        wstring partOfSpeech = categoryJson[L"partOfSpeech"].as_string();
        wstring baseForm = categoryJson[L"baseForm"].as_string();

        vector<TranslateResultCategoryEntry> translateResultDictionaryEntries;
        json::array entriesJson = categoryJson[L"entries"].as_array();
        for (size_t j = 0; j < entriesJson.size(); ++j)
        {
            json::value entryJson = entriesJson[j];
            wstring word = entryJson[L"word"].as_string();
            double score = entryJson[L"score"].as_double();

            vector<wstring> reverseTransltions;
            json::array reverseTranslationsJson = entryJson[L"reverseTranslations"].as_array();
            for (size_t k = 0; k < reverseTranslationsJson.size(); ++k)
            {
                reverseTransltions.push_back(reverseTranslationsJson[k].as_string());
            }

            translateResultDictionaryEntries.push_back(TranslateResultCategoryEntry(word, reverseTransltions, score));
        }

        categories.push_back(TranslateResultCategory(partOfSpeech, baseForm, false, translateResultDictionaryEntries));
    }

    return categories;
}

TranslateResult::~TranslateResult()
{
}
