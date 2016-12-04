#include "Services\Translation\Dto\TranslateResult.h"
#include "Utilities\StringUtilities.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateException.h"

const char* TranslateResult::SentenceKey = "sentence";
const char* TranslateResult::CategoriesKey = "categories";

const char* TranslateResult::TranslationKey = "translation";
const char* TranslateResult::OriginKey = "origin";
const char* TranslateResult::InputKey = "input";
const char* TranslateResult::SuggestionKey = "suggestion";

const char* TranslateResult::EntriesKey = "entries";
const char* TranslateResult::BaseFormKey = "baseForm";
const char* TranslateResult::PartOfSpeechKey = "partOfSpeech";
const char* TranslateResult::WordKey = "word";
const char* TranslateResult::ScoreKey = "score";
const char* TranslateResult::ReverseTranslationsKey = "reverseTranslations";

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
        { TranslationKey, StringUtilities::GetUtf8String(translateResult.GetSentence().GetTranslation()) },
        { OriginKey, StringUtilities::GetUtf8String(translateResult.GetSentence().GetOrigin()) },
        { InputKey, StringUtilities::GetUtf8String(translateResult.GetSentence().GetInput()) },
        { SuggestionKey, StringUtilities::GetUtf8String(translateResult.GetSentence().GetSuggestion()) }
    };

    json categoriesJson;
    for (size_t i = 0; i < translateResult.GetTranslateCategories().size(); ++i)
    {
        TranslateResultCategory category = translateResult.GetTranslateCategories()[i];
        json categoryJson;
        categoryJson[BaseFormKey] = StringUtilities::GetUtf8String(category.GetBaseForm());
        categoryJson[PartOfSpeechKey] = StringUtilities::GetUtf8String(category.GetPartOfSpeech());

        json categoryEntriesJson;
        for (size_t j = 0; j < category.GetEntries().size(); ++j)
        {
            TranslateResultCategoryEntry categoryEntry = category.GetEntries()[j];
            json categoryEntryJson;
            categoryEntryJson[WordKey] = StringUtilities::GetUtf8String(categoryEntry.GetWord());
            categoryEntryJson[ScoreKey] = categoryEntry.GetScore();

            json reverseTranslationsJson;
            for (size_t k = 0; k < categoryEntry.GetReverseTranslations().size(); ++k)
            {
                reverseTranslationsJson[k] = StringUtilities::GetUtf8String(categoryEntry.GetReverseTranslations()[k]);
            }

            categoryEntryJson[ReverseTranslationsKey] = reverseTranslationsJson;

            categoryEntriesJson[j] = categoryEntryJson;
        }

        categoryJson[EntriesKey] = categoryEntriesJson;

        categoriesJson[i] = categoryJson;
    }

    json translateResultJson;
    translateResultJson[SentenceKey] = sentenceJson;
    translateResultJson[CategoriesKey] = categoriesJson;

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

TranslateResultSentence TranslateResult::ParseTranslateResultSentence(json root)
{
    json sentenceJson = root[SentenceKey];

    wstring translation = StringUtilities::GetJsonString(sentenceJson[TranslationKey]);
    wstring origin = StringUtilities::GetJsonString(sentenceJson[OriginKey]);
    wstring input = StringUtilities::GetJsonString(sentenceJson[InputKey]);
    wstring suggestion = StringUtilities::GetJsonString(sentenceJson[SuggestionKey]);

    return TranslateResultSentence(translation, origin, input, suggestion);
}

vector<TranslateResultCategory> TranslateResult::ParseTranslateCategories(json root)
{
    vector<TranslateResultCategory> categories;

    if (!root[CategoriesKey].is_array())
    {
        return categories;
    }

    json categoriesJson = root[CategoriesKey];

    for (size_t i = 0; i < categoriesJson.size(); ++i)
    {
        json categoryJson = categoriesJson[i];

        wstring partOfSpeech = StringUtilities::GetJsonString(categoryJson[PartOfSpeechKey]);
        wstring baseForm = StringUtilities::GetJsonString(categoryJson[BaseFormKey]);

        vector<TranslateResultCategoryEntry> translateResultDictionaryEntries;
        json entriesJson = categoryJson[EntriesKey];
        for (size_t j = 0; j < entriesJson.size(); ++j)
        {
            json entryJson = entriesJson[j];
            wstring word = StringUtilities::GetJsonString(entryJson[WordKey]);
            double score = entryJson[ScoreKey].get<double>();

            vector<wstring> reverseTransltions;
            json reverseTranslationsJson = entryJson[ReverseTranslationsKey];
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
