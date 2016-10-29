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

TranslateResult::~TranslateResult()
{
}
