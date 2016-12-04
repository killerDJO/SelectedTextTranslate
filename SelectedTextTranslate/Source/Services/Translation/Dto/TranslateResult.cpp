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
    json sentenceJson = 
    {
        { "translation", StringUtilities::GetUtf8String(translateResult.GetSentence().GetTranslation()) },
        { "origin", StringUtilities::GetUtf8String(translateResult.GetSentence().GetOrigin()) },
        { "input", StringUtilities::GetUtf8String(translateResult.GetSentence().GetInput()) },
        { "suggestion", StringUtilities::GetUtf8String(translateResult.GetSentence().GetSuggestion()) }
    };

    json categoriesJson;
    for (size_t i = 0; i < translateResult.GetTranslateCategories().size(); ++i)
    {
        TranslateResultCategory category = translateResult.GetTranslateCategories()[i];
        json categoryJson;
        categoryJson["baseForm"] = StringUtilities::GetUtf8String(category.GetBaseForm());
        categoryJson["partOfSpeech"] = StringUtilities::GetUtf8String(category.GetPartOfSpeech());

        json categoryEntriesJson;
        for (size_t j = 0; j < category.GetEntries().size(); ++j)
        {
            TranslateResultCategoryEntry categoryEntry = category.GetEntries()[j];
            json categoryEntryJson;
            categoryEntryJson["word"] = StringUtilities::GetUtf8String(categoryEntry.GetWord());
            categoryEntryJson["score"] = categoryEntry.GetScore();

            json reverseTranslationsJson;
            for (size_t k = 0; k < categoryEntry.GetReverseTranslations().size(); ++k)
            {
                reverseTranslationsJson[k] = StringUtilities::GetUtf8String(categoryEntry.GetReverseTranslations()[k]);
            }

            categoryEntryJson["reverseTranslations"] = reverseTranslationsJson;

            categoryEntriesJson[j] = categoryEntryJson;
        }

        categoryJson["entries"] = categoryEntriesJson;

        categoriesJson[i] = categoryJson;
    }

    json translateResultJson;
    translateResultJson["sentence"] = sentenceJson;
    translateResultJson["categories"] = categoriesJson;

    string json = translateResultJson.dump();
    return StringUtilities::GetUtf16String(json);
}

TranslateResult TranslateResult::ParseFromJson(wstring json)
{
    nlohmann::json root = json::parse(StringUtilities::GetUtf8String(json).c_str());

    if (root.is_null())
    {
        return TranslateResult();
    }

    TranslateResultSentence sentence = ParseTranslateResultSentence(root);

    vector<TranslateResultCategory> categories = ParseTranslateCategories(root);

    return TranslateResult(sentence, categories);
}

TranslateResultSentence TranslateResult::ParseTranslateResultSentence(nlohmann::json root)
{
    nlohmann::json sentenceJson = root["sentence"];

    wstring translation = StringUtilities::GetJsonString(sentenceJson["translation"]);
    wstring origin = StringUtilities::GetJsonString(sentenceJson["origin"]);
    wstring input = StringUtilities::GetJsonString(sentenceJson["input"]);
    wstring suggestion = StringUtilities::GetJsonString(sentenceJson["suggestion"]);

    return TranslateResultSentence(translation, origin, input, suggestion);
}

vector<TranslateResultCategory> TranslateResult::ParseTranslateCategories(json root)
{
    vector<TranslateResultCategory> categories;

    if (!root["categories"].is_array())
    {
        return categories;
    }

    json categoriesJson = root["categories"];

    for (size_t i = 0; i < categoriesJson.size(); ++i)
    {
        json categoryJson = categoriesJson[i];

        wstring partOfSpeech = StringUtilities::GetJsonString(categoryJson["partOfSpeech"]);
        wstring baseForm = StringUtilities::GetJsonString(categoryJson["baseForm"]);

        vector<TranslateResultCategoryEntry> translateResultDictionaryEntries;
        json entriesJson = categoryJson["entries"];
        for (size_t j = 0; j < entriesJson.size(); ++j)
        {
            json entryJson = entriesJson[j];
            wstring word = StringUtilities::GetJsonString(entryJson["word"]);
            double score = entryJson["score"].get<double>();

            vector<wstring> reverseTransltions;
            json reverseTranslationsJson = entryJson["reverseTranslations"];
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

TranslateResult::~TranslateResult()
{
}
